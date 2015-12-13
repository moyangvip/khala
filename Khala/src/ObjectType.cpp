/*
 * ObjectType.cpp
 *
 *  Created on: Dec 8, 2015
 *      Author: moss
 */
#include <khala/NodeServer.h>
#include <khala/ObjectType.h>
#include <khala/ParseKey.h>
using namespace khala;
void ObjectType::setNodeServer(NodeServer* nodeServer) {
	nodeServer_ = nodeServer;
	nodeManager_.setNodeServer(nodeServer);
}
ObjectType* ObjectType::getObjectType(const std::string& objectType){
	return nodeServer_->getObjectType(objectType);
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
