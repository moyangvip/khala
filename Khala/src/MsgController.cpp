/*
 * MessageHandler.cpp
 *
 *  Created on: Dec 3, 2015
 *      Author: moss
 */
#include <khala/MsgController.h>
#include <khala/NodeServer.h>
#include <khala/NodeType.h>
#include <khala/ParseKey.h>
#include <muduo/base/Logging.h>
using namespace khala;
void MsgController::onDispatcher(const TcpConnectionPtr& conn, Json::Value& msg,
		Timestamp time, std::string& typeStr) {
	InfoNodePtr infoNodePtr = getConnNode(conn);
	//try to match the type handler
	ObjectType* objectType = 0;
	getObjectType(infoNodePtr->getNodeType(), &objectType);
	bool runRes = true;
	bool searchRes = true;
	//first to find handler from curr objectType,if can't find,search handler from TempNodeType
	while (searchRes) {
		MsgHandlerMap& handler = objectType->getRegisterHandler();
		MsgHandlerMap::iterator it = handler.find(typeStr);
		if (it != handler.end()) {
			//try to run before msg function
			if (onBeforeMessage(infoNodePtr, msg, time,
					objectType->getObjectTypeName())) {
				//run the real msg function
				if ((it->second)(infoNodePtr, msg, time)) {
					//try to update tmp conn to pool head
					if (infoNodePtr->getStatus() == NO_LOGIN_STATUS) {
						uint tmpId = TMP_ID_CONN(conn);
						nodeServer_->getTempConnectPool()->updateTempConnect(
								tmpId);
					}
				} else {
					//handler run err
					runRes = false;
				}
			}
			break;
		} else {
			//try to find handler from TempNodeType
			if (objectType->getObjectTypeName() != TEMP_NODE_TYPE) {
				getObjectType(TEMP_NODE_TYPE, &objectType);
			} else {
				searchRes = false;
			}
		}
	}
	if (!searchRes) {
		//can't find this type handler
		onErrTypeMessage(infoNodePtr, msg, time);
	} else {
		if (!runRes) {
			//if run false...
			onErrRunMessage(conn, msg, time);
		}
	}

}
/*
 * do login check
 * exclude TEMP_NODE_TYPE and NODE_TYPE'sLOGIN_TYPE
 * */
bool MsgController::onBeforeMessage(InfoNodePtr& infoNodePtr, Json::Value& msg,
		Timestamp time, const std::string& realHandlerType) {
	if (realHandlerType == TEMP_NODE_TYPE) {
		return true;
	}
	return onBeforeMessage_(infoNodePtr, msg, time);
}
//do some check before msg,for example: is login?
bool MsgController::onBeforeMessage_(InfoNodePtr& infoNodePtr, Json::Value& msg,
		Timestamp time) {
	if (infoNodePtr->getStatus() != LOGIN_STATUS) {
		TcpConnectionPtr& conn = infoNodePtr->getConn();
		LOG_ERROR<<conn->peerAddress().toIpPort()<<" temp ID:"<<TMP_ID_CONN(conn)<<" NO LOGIN Type:"<<msg[MSG_TYPE].asString();
		ObjectType* objectType = 0;
		this->getObjectType(TEMP_NODE_TYPE, &objectType);
		/*
		 * only when no login type requst logout will invoke onNoLoginMessage,
		 * so remove it and unify as  onErrTypeMessage
		 * */
		//objectType->onNoLoginMessage(infoNodePtr, msg, time);
		objectType->onErrTypeMessage(infoNodePtr, msg, time);
		return false;
	}
	return true;
}
void MsgController::onConnection(const TcpConnectionPtr& conn,
		Timestamp time) {
	ConnNodePtr tempNodePtr(new ConnNode(conn, time));
	if (nodeServer_->getTempConnectPool()->push_front(tempNodePtr)) {
		LOG_INFO << conn->peerAddress().toIpPort() << " tmp ID:"
				<< tempNodePtr->getTmpId() << " create!";
	}
}
void MsgController::onDisConnection(const TcpConnectionPtr& conn,
		Timestamp time) {
//try to remove from tmp pools
	nodeServer_->getTempConnectPool()->remove(TMP_ID_CONN(conn));
	uint id = ID_CONN(conn);
	InfoNodePtr connNodePtr;
	if (nodeServer_->getNodePool()->find(id, connNodePtr)) {
		//abnormal exit
		onAbnormalExit(conn, connNodePtr, time);
	}
}
//can not send anything
void MsgController::onAbnormalExit(const TcpConnectionPtr& conn,
		InfoNodePtr& infoNodePtr, Timestamp time) {
	uint id = ID_CONN(conn);
	LOG_ERROR<<conn->peerAddress().toIpPort()<<" ID:"<<id<<" abnormal exit!";
	ObjectType* objectType = 0;
	getObjectType(infoNodePtr->getNodeType(), &objectType);
	((NodeType*) objectType)->releaseConnNode(infoNodePtr, time);
	nodeServer_->getNodePool()->remove(id);
}
void MsgController::onErrTypeMessage(InfoNodePtr& infoNodePtr, Json::Value& msg,
		Timestamp time) {
	TcpConnectionPtr& conn = infoNodePtr->getConn();
	uint id = ID_CONN(conn);
	if (id != DEFAULT_ID) {
		LOG_ERROR<<conn->peerAddress().toIpPort()<<" ID:"<<id<<" err Type:"<<msg[MSG_TYPE].asString();
	} else {
		LOG_ERROR<<conn->peerAddress().toIpPort()<<" temp ID:"<<TMP_ID_CONN(conn)<<" err Type:"<<msg[MSG_TYPE].asString();
	}
	ObjectType* objectType = 0;
	getObjectType(infoNodePtr->getNodeType(), &objectType);
	objectType->onErrTypeMessage(infoNodePtr, msg, time);
}
void MsgController::onErrRunMessage(const TcpConnectionPtr& conn,
		Json::Value& msg, Timestamp time) {
	uint id = ID_CONN(conn);
	if (id != DEFAULT_ID) {
		LOG_ERROR<<conn->peerAddress().toIpPort()<<" ID:"<<ID_CONN(conn)<<" ErrRun,msg:"<<fastWriter.write(msg);
	} else {
		LOG_ERROR<<conn->peerAddress().toIpPort()<<" temp ID:"<<TMP_ID_CONN(conn)<<" ErrRun,msg:"<<fastWriter.write(msg);
	}
	//conn->send("err,running err!");
}

bool MsgController::isLogin(const TcpConnectionPtr& conn, Json::Value& msg,
		Timestamp time) {
	uint id = ID_CONN(conn);
	if (!nodeServer_->getNodePool()->hasNode(id)) {
		LOG_ERROR<<conn->peerAddress().toIpPort()<<" temp ID:"<<TMP_ID_CONN(conn)<<" No Login Type REQ!";
		return false;
	}
	return true;
}
InfoNodePtr MsgController::getConnNode(const TcpConnectionPtr& conn) {
	uint id = ID_CONN(conn);
	InfoNodePtr infoNodePtr;
	if (!nodeServer_->getNodePool()->find(id, infoNodePtr)) {
		//no login conn,try to find from tmpPool
		uint tmpId = TMP_ID_CONN(conn);
		ConnNodePtr connNodePtr;
		if (nodeServer_->getTempConnectPool()->find(tmpId, connNodePtr)) {
			InfoNodePtr tmpPtr(
					new InfoNode(connNodePtr,Timestamp::now()));
			infoNodePtr.swap(tmpPtr);
		}
	}
	return infoNodePtr;
}
bool MsgController::addObjectType(const std::string& objectTypeStr,
		ObjectType* objectType) {
	ObjectTypeMap::iterator it = objectTypeMap_.find(objectTypeStr);
	if (it != objectTypeMap_.end()) {
		LOG_ERROR<<"\""<<objectTypeStr<<"\""<<" has set as a other type! please set a new one!";
		return false;
	}
	objectTypeMap_[objectTypeStr] = objectType;
	LOG_INFO<<"Add "<<objectTypeStr<<" as Server dev Type success!";
	return true;
}
bool MsgController::getObjectType(const std::string& objectTypeStr,
		ObjectType** objectType) {
	ObjectTypeMap::iterator it = objectTypeMap_.find(objectTypeStr);
	if (it != objectTypeMap_.end()) {
		(*objectType) = it->second;
		return true;
	}
	//if can't find objectTypeStr,try to set as TEMP_NODE_TYPE
	it = objectTypeMap_.find(TEMP_NODE_TYPE);
	if (it != objectTypeMap_.end()) {
		(*objectType) = it->second;
	}
	return false;
}
void MsgController::setTempNodeType(TempNodeType* tempNodeType) {
	objectTypeMap_[TEMP_NODE_TYPE] = tempNodeType;
}
