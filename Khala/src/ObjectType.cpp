/*
 * ObjectType.cpp
 *
 *  Created on: Dec 8, 2015
 *      Author: moss
 */
#include <khala/NodeServer.h>
#include <khala/ObjectType.h>
#include <muduo/base/Logging.h>
using namespace khala;
RegisterHandler::RegisterHandler(MsgHandlerMap& msgHandlerMap) :
		msgHandlerMap_(msgHandlerMap) {
}
bool RegisterHandler::setRegisterMsg(const std::string & type,
		const RegisterMessageCallback& cb) {
	//LOGIN_TYPE and LOGOUT_TYPE is Reserved words,they will be register by NodeType and you can't modify it
	if (type == LOGIN_TYPE || type == LOGOUT_TYPE) {
		MsgHandlerMap::iterator it = msgHandlerMap_.find(type);
		if (it != msgHandlerMap_.end()) {
			return false;
		}
	}
	msgHandlerMap_[type] = cb;
	return true;
}
void ObjectType::setNodeServer(NodeServer* nodeServer) {
	nodeServer_ = nodeServer;
	nodeManager_.setNodeServer(nodeServer);
}
ObjectType* ObjectType::getObjectType(const std::string& objectType) {
	return nodeServer_->getObjectType(objectType);
}
ObjectType::ObjectType() :
		nodeServer_(0) {
}
ObjectType::~ObjectType() {
}
ObjectType* ObjectType::getInstance() {
	return (ObjectType *) 0;
}
MsgHandlerMap& ObjectType::getRegisterHandler() {
	return msgHandlerMap_;
}
NodeManager* ObjectType::getNodeManager() {
	return &nodeManager_;
}
void ObjectType::setRegisterMsg_() {
	RegisterHandler registerHandler(msgHandlerMap_);
	setRegisterMsg(registerHandler);
}
/*
 * if receive any no register type msg with this nodeType,will invoke this function
 * */
void ObjectType::onErrTypeMessage(InfoNodePtr& infoNodePtr, Json::Value& msg,
		Timestamp time) {
	Json::Value res;
	res[MSG_TYPE] = ERR_TYPE;
	res[DATA] = "err type req!";
	Json::FastWriter jwriter;
	std::string sendStr = jwriter.write(res);
	infoNodePtr->send(sendStr);
}
/*
 * if  any msg handler return false,will invoke this function
 * */
void ObjectType::onErrRunMessage(InfoNodePtr& infoNodePtr, Json::Value& msg,
		Timestamp time) {
	Json::Value res;
	res[MSG_TYPE] = ERR_RUN;
	res[DATA] = "err,running err!";
	Json::FastWriter jwriter;
	std::string sendStr = jwriter.write(res);
	infoNodePtr->send(sendStr);
}
void ObjectType::onOverTime(InfoNodePtr& infoNodePtr, Timestamp time) {
	Json::Value res;
	res[MSG_TYPE] = OVER_TIME;
	res[DATA] = "err,connect overtime!";
	Json::FastWriter jwriter;
	std::string sendStr = jwriter.write(res);
	infoNodePtr->send(sendStr);
}
