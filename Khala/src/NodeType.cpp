/*
 * NodeType.cpp
 *
 *  Created on: Dec 8, 2015
 *      Author: moss
 */
#include <khala/NodeType.h>
#include <khala/NodeServer.h>
#include <muduo/base/Logging.h>
#include <sstream>
using namespace khala;
bool NodeType::onLogoutMsg_(InfoNodePtr& infoNodePtr, Json::Value& msg,
		Timestamp time) {
	muduo::net::TcpConnectionPtr& conn = infoNodePtr->getConn();
	uint id = ID_CONN(conn);
	if (!onLogoutMsg(infoNodePtr, msg, time)) {
		return false;
	}
	releaseConnNode(infoNodePtr, time);
	nodeServer_->getNodePool()->forceRemove(id, infoNodePtr->getNodeType());
	infoNodePtr->setStatus(NO_LOGIN_STATUS);
	LOG_INFO << conn->peerAddress().toIpPort() << " ID:" << ID_CONN(conn)
			<< " logout success!";
	return true;
}

bool NodeType::onNodeIdMsg(InfoNodePtr& infoNodePtr, Json::Value& msg,
		Timestamp time) {
	uint id = infoNodePtr->getId();
	std::stringstream ss;
	ss << "Node id:" << id;
	Json::Value res;
	res[MSG_TYPE] = NODE_ID_TYPE;
	res[DATA] = ss.str();
	Json::FastWriter jwriter;
	std::string sendStr = jwriter.write(res);
	infoNodePtr->send(sendStr);
	return true;
}
NodeType::NodeType() {
}
NodeType::~NodeType() {
}
NodeType* NodeType::getInstance() {
	static NodeType nodeType;
	return &nodeType;
}
/*
 * you can do sth as logining checking login with databases,
 * you can invoke InfoNodePtr->setId() and set a UNIQUE id as the conn key
 * or default use connNode's tmpID as id key(i promise login success:->)
 * if check false,just return false,and onLoginSuccessMsg will not be invoked
 * if you return true,not sure login success,if you set node id is not unique,logining will be failure
 * */
bool NodeType::onLoginCheckMsg(InfoNodePtr&, Json::Value& msg, Timestamp time) {
	return true;
}
/*
 * if you login success...
 * */
bool NodeType::onLoginSuccessMsg(InfoNodePtr& infoNodePtr, Json::Value& msg,
		Timestamp time) {
	Json::Value res;
	res[MSG_TYPE] = LOGIN_TYPE;
	res[RESULT] = "true";
	res[DATA] = "login success!";
	Json::FastWriter jwriter;
	std::string sendStr = jwriter.write(res);
	infoNodePtr->send(sendStr);
	return true;
}
/*
 * if you login failure,such as logined id
 * */
bool NodeType::onLoginFailureMsg(InfoNodePtr& infoNodePtr, Json::Value& msg,
		Timestamp time) {
	Json::Value res;
	res[MSG_TYPE] = LOGIN_TYPE;
	res[RESULT] = "false";
	res[DATA] = "err,logined id!";
	Json::FastWriter jwriter;
	std::string sendStr = jwriter.write(res);
	infoNodePtr->send(sendStr);
	return true;
}
/*
 * you can do sth when logout
 * after this function,will invoke destroyConnNode() to release node resources
 * */
bool NodeType::onLogoutMsg(InfoNodePtr& infoNodePtr, Json::Value& msg,
		Timestamp time) {
	Json::Value res;
	res[MSG_TYPE] = LOGOUT_TYPE;
	res[RESULT] = "true";
	res[DATA] = "logout success!";
	Json::FastWriter jwriter;
	std::string sendStr = jwriter.write(res);
	infoNodePtr->send(sendStr);
	return true;
}
/*
 * destroy node resources.
 * if you do not set any node resources,do nothing
 * can not use InfoNodePtr->getConn() to send sth;
 * */
void NodeType::releaseConnNode(InfoNodePtr& infoNodePtr, Timestamp time) {

}
/*
 * you can register msg at here
 * for example:
 * you can create a function as onLoginMsg:
 * bool onLoginMsg(ConnNodePtr& connNode, Json::Value& msg,muduo::Timestamp time)
 * and you can register this function like:
 * handler.setRegisterMsg(LOGIN_TYPE, boost::bind(&NodeType::onLoginMsg, this, _1, _2, _3));
 * when you receive a msg type as LOGIN_TYPE,the function setRegisterMsg will be invoking
 * Notice:LOGIN_TYPE and LOGOUT_TYPE are Reserved words,you can't set as TYPE str
 * */
void NodeType::setRegisterMsg(RegisterHandler& handler) {
	handler.setRegisterMsg(LOGOUT_TYPE,
			boost::bind(&NodeType::onLogoutMsg_, this, _1, _2, _3));
	handler.setRegisterMsg(NODE_ID_TYPE,
			boost::bind(&NodeType::onNodeIdMsg, this, _1, _2, _3));
}
/*
 * you must set a unique string type as the NodeType's key!
 */
const std::string& NodeType::getObjectTypeName() {
	static std::string typeStr(LOGIN_NODE_TYPE);
	return typeStr;
}
