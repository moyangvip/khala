/*
 * MessageHandler.cpp
 *
 *  Created on: Dec 3, 2015
 *      Author: moss
 */
#include <khala/MsgController.h>
#include <khala/NodeServer.h>
#include <khala/NodeType.h>
#include <khala/TempNodeType.h>
#include <khala/ParseKey.h>
#include <muduo/base/Logging.h>
using namespace khala;
void MsgController::onDispatcher(const TcpConnectionPtr& conn, Json::Value& msg,
		Timestamp time, std::string& typeStr) {
	InfoNodePtr infoNodePtr = getInfoNode(conn);
	//if infoNodePtr is overtime,abandon this msg...
	if (infoNodePtr->getStatus() == OVERTIME) {
		LOG_ERROR<<conn->peerAddress().toIpPort()<<" ID:"<<ID_CONN(conn)<<" temp ID:"<<TMP_ID_CONN(conn)<<" is OVERTIME,receive msg will be Abandoned!";
		return;
	}
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
					updateAliveTime(infoNodePtr);
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
void MsgController::onConnection(const TcpConnectionPtr& conn, Timestamp time) {
	InfoNodePtr infoNodePtr(new InfoNode(conn));
	if (nodeServer_->getTempNodePool()->setNewNode(infoNodePtr->getTempId(),
			infoNodePtr)) {
		LOG_INFO << conn->peerAddress().toIpPort() << " tmp ID:"
				<< infoNodePtr->getTempId() << " create!";
	}
}
void MsgController::onDisConnection(const TcpConnectionPtr& conn,
		Timestamp time) {
	//try to remove from tmp pools
	nodeServer_->getTempNodePool()->remove(TMP_ID_CONN(conn));
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
	nodeServer_->getNodePool()->forceRemove(id, infoNodePtr->getNodeType());
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
void MsgController::onOverTime(InfoNodePtr& infoNodePtr, Timestamp time) {
	TcpConnectionPtr conn = infoNodePtr->getConn();
	LOG_INFO << conn->peerAddress().toIpPort() << "  ID:"
			<< infoNodePtr->getId() << " temp ID:" << infoNodePtr->getTempId()
			<< " devType:" << infoNodePtr->getNodeType()
			<< " is OVERTIME,will be disconnect!";

	ObjectType* objectType = 0;
	getObjectType(infoNodePtr->getNodeType(), &objectType);
	//run in conn's thread loop
	conn->getLoop()->runInLoop(
			boost::bind(&ObjectType::onOverTime, objectType, infoNodePtr,
					time));
	//try to remove from nodePool
	nodeServer_->getTempNodePool()->remove(infoNodePtr->getTempId());
	nodeServer_->getNodePool()->forceRemove(infoNodePtr->getId(),
			infoNodePtr->getNodeType());
}
InfoNodePtr MsgController::getInfoNode(const TcpConnectionPtr& conn) {
	uint id = ID_CONN(conn);
	InfoNodePtr infoNodePtr;
	std::string type = TYPE_CONN(conn);
	if (type == TEMP_NODE_TYPE || type == "") {
		//try to find from tmpPool
		uint tmpId = TMP_ID_CONN(conn);
		if (!nodeServer_->getTempNodePool()->find(tmpId, infoNodePtr)) {
			LOG_ERROR<<conn->peerAddress().toIpPort()<<" ID:"<<ID_CONN(conn)<<" temp ID:"<<TMP_ID_CONN(conn)<<" Can't find InfoNode!";
		}
	} else {
		//first try to find from this type node pool
		if (!nodeServer_->getNodePool()->find(id, infoNodePtr, type)) {
			//no login conn,try to find from tmpPool
			uint tmpId = TMP_ID_CONN(conn);
			if (!nodeServer_->getTempNodePool()->find(tmpId, infoNodePtr)) {
				LOG_ERROR<<conn->peerAddress().toIpPort()<<" ID:"<<ID_CONN(conn)<<" temp ID:"<<TMP_ID_CONN(conn)<<" Can't find InfoNode!";
			}
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
	LOG_INFO << "Add " << objectTypeStr << " as Server dev Type success!";
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
void MsgController::updateAliveTime(InfoNodePtr& infoNodePtr) {
	if (infoNodePtr->getStatus() == NO_LOGIN_STATUS) {
		uint tmpId = infoNodePtr->getTempId();
		nodeServer_->getTempNodePool()->updateAliveId(tmpId);
	} else {
		uint id = infoNodePtr->getId();
		nodeServer_->getNodePool()->updateAliveId(id);
	}
}
