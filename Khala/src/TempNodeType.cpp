/*
 * TempNodeType.cpp
 *
 *  Created on: Dec 8, 2015
 *      Author: moss
 */
#include <khala/TempNodeType.h>
#include <khala/ConnID.h>
#include <khala/NodeServer.h>
#include <khala/NodeType.h>
#include <muduo/base/Logging.h>

using namespace khala;
TempNodeType::TempNodeType() {
}
TempNodeType::~TempNodeType() {
}
TempNodeType* TempNodeType::getInstance() {
	static TempNodeType tempNodeType;
	return &tempNodeType;
}

const std::string& TempNodeType::getObjectTypeName() {
	static std::string typeStr(TEMP_NODE_TYPE);
	return typeStr;
}
bool TempNodeType::onLoginMsg_(InfoNodePtr& infoNodePtr, Json::Value& msg,
		Timestamp time) {
	muduo::net::TcpConnectionPtr& conn = infoNodePtr->getConn();
	if (infoNodePtr->getStatus() == LOGIN_STATUS) {
		//has logined
		LOG_WARN<< conn->peerAddress().toIpPort() << " ID:"
		<< infoNodePtr->getId() << " reloading";
		ObjectType* objectType = nodeServer_->getObjectType(
				infoNodePtr->getNodeType());
		NodeType* nodeType = dynamic_cast<NodeType*>(objectType);
		return nodeType->onLoginSuccessMsg(infoNodePtr, msg, time);
	}
	ObjectType* objectType = onLoginTypeCheck(infoNodePtr, msg);
	//because onLoginTypeCheck will always set as NodeType...
	NodeType* nodeType = dynamic_cast<NodeType*>(objectType);
	if (nodeType == 0)
		return false;
	if (!nodeType->onLoginCheckMsg(infoNodePtr, msg, time)) {
		LOG_ERROR<< conn->peerAddress().toIpPort() << "tmp ID:"
		<<infoNodePtr->getTempId()<< " login failure!";
		return true;
	}
	//set id
	uint id = infoNodePtr->getId();
	if (id == DEFAULT_ID) {
		LOG_INFO << conn->peerAddress().toIpPort()
				<< " ID will be set as tmp ID:" << infoNodePtr->getTempId();
		id = infoNodePtr->getTempId();
		infoNodePtr->setId(id);
	}
	if (!nodeServer_->getNodePool()->hasNode(id)) {
		//try to set new type
		std::string oldTypeStr = infoNodePtr->getNodeType();
		infoNodePtr->setNodeType(nodeType);
		//the new id has not login,add to nodePool
		if (nodeServer_->getNodePool()->setNewNode(id, infoNodePtr)) {
			//add to nodePool success,remove from TempNodePool
			nodeServer_->getTempNodePool()->remove(infoNodePtr->getTempId());

			infoNodePtr->setStatus(LOGIN_STATUS);
			LOG_INFO << conn->peerAddress().toIpPort() << " ID:" << id
					<< " login success!";
			return nodeType->onLoginSuccessMsg(infoNodePtr, msg, time);
		} else {
			//loading err,can't set id,may other threading is login with the id
			//reset as old type
			ObjectType* oldType;
			nodeServer_->getMsgController().getObjectType(oldTypeStr, &oldType);
			infoNodePtr->setNodeType(oldType);

			LOG_ERROR<< conn->peerAddress().toIpPort() << " ID:" << id
			<< " login err!";
			bool res = nodeType->onLoginFailureMsg(infoNodePtr, msg, time);
			//reset id as DEFAULT_ID
			infoNodePtr->setId(DEFAULT_ID);
			return res;
		}
	} else {
		//the id has login
		LOG_ERROR<< conn->peerAddress().toIpPort() << " ID:" << id
		<< " Has logined id!";
		bool res = nodeType->onLoginFailureMsg(infoNodePtr, msg, time);
		//reset id as DEFAULT_ID
		infoNodePtr->setId(DEFAULT_ID);
		return res;
	}
	return true;
}
ObjectType* TempNodeType::onLoginTypeCheck(InfoNodePtr& infoNodePtr,
		Json::Value& msg) {
	TcpConnectionPtr& conn = infoNodePtr->getConn();
	std::string nodeTypeStr = msg[NODE_TYPE].asString();
	//set nodeType as the default nodetype
	ObjectType* nodeType = 0;
	if (nodeTypeStr == TEMP_NODE_TYPE) {
		LOG_ERROR<< conn->peerAddress().toIpPort() << " temp ID:"
		<< infoNodePtr->getTempId()
		<< " can't set Node Type as TEMP_NODE_TYPE!";
		nodeServer_->getMsgController().getObjectType(LOGIN_NODE_TYPE,
				&nodeType);
	} else {
		if (nodeTypeStr != "") {
			if (!nodeServer_->getMsgController().getObjectType(nodeTypeStr,
							&nodeType)) {
				LOG_ERROR << conn->peerAddress().toIpPort() << " temp ID:"
				<<infoNodePtr->getTempId()<< " err Node Type:" << nodeTypeStr
				<< "!";
				nodeServer_->getMsgController().getObjectType(LOGIN_NODE_TYPE,
						&nodeType);
			}
		} else {
			LOG_ERROR << conn->peerAddress().toIpPort() << " temp ID:"
			<<infoNodePtr->getTempId() << " No Set  Login Type!";
			nodeServer_->getMsgController().getObjectType(LOGIN_NODE_TYPE,
					&nodeType);
		}
	}
	LOG_INFO << conn->peerAddress().toIpPort() << " temp ID:"
			<< infoNodePtr->getTempId() << " set as\""
			<< nodeType->getObjectTypeName() << "\" TYPE!";
	return nodeType;
}
void TempNodeType::setRegisterMsg(RegisterHandler& handler) {
	handler.setRegisterMsg(LOGIN_TYPE,
			boost::bind(&TempNodeType::onLoginMsg_, this, _1, _2, _3));
	handler.setRegisterMsg(DEV_TYPE,
			boost::bind(&TempNodeType::onDevTypeMsg, this, _1, _2, _3));
	handler.setRegisterMsg(IS_LOGIN_TYPE,
			boost::bind(&TempNodeType::onIsLoginMsg, this, _1, _2, _3));
	handler.setRegisterMsg(HEART_TYPE,
			boost::bind(&TempNodeType::onHeartMsg, this, _1, _2, _3));
}
bool TempNodeType::onDevTypeMsg(InfoNodePtr& infoNodePtr, Json::Value& msg,
		Timestamp time) {
	Json::Value res;
	res[MSG_TYPE] = DEV_TYPE;
	std::string devType = infoNodePtr->getNodeType();
	res[DATA] = "devType:" + devType;
	Json::FastWriter jwriter;
	std::string sendStr = jwriter.write(res);
	infoNodePtr->send(sendStr);
	return true;
}
void TempNodeType::onCreate(InfoNodePtr& infoNodePtr, Timestamp time) {
	return;
}
bool TempNodeType::onHeartMsg(InfoNodePtr& infoNodePtr, Json::Value& msg,
		Timestamp time) {
	return true;
}
bool TempNodeType::onIsLoginMsg(InfoNodePtr& infoNodePtr, Json::Value& msg,
		Timestamp time) {
	Json::Value res;
	res[MSG_TYPE] = IS_LOGIN_TYPE;
	if (infoNodePtr->getStatus() == NO_LOGIN_STATUS) {
		res[DATA] = "Login status: off";
	} else {
		res[DATA] = "Login status: on";
	}
	Json::FastWriter jwriter;
	std::string sendStr = jwriter.write(res);
	infoNodePtr->send(sendStr);
	return true;
}

