/*
 * NodeServer.cpp
 *
 *  Created on: Dec 3, 2015
 *      Author: moss
 */
#include <khala/NodeServer.h>
#include <khala/Config.h>
#include <khala/HeadCode.h>
#include <khala/ParseKey.h>
#include <khala/NodeType.h>
#include <json/json.h>
#include <boost/bind.hpp>
#include <muduo/base/Logging.h>
using namespace khala;
NodeServer::NodeServer(const InetAddress& listenAddr) :
		server_(&loop_, listenAddr, "UsrServer"), idleAliveTime_(
		DEFAULT_IDLE_ALIVE_TIME), checkIdleTime_(
		DEFAULT_CHECK_IDLE_TIME), msgController_(this) {
	tempConnectPool_.setIdleAliveTime(idleAliveTime_);
	setTempNodeTypeInstance(TempNodeType::getInstance());
	this->addNodeType(NodeType::getInstance());
	server_.setConnectionCallback(
			boost::bind(&NodeServer::onConnection, this, _1));
	server_.setMessageCallback(
			boost::bind(&NodeServer::onMessage, this, _1, _2, _3));
}
NodeServer::~NodeServer() {
}
void NodeServer::start(int usrIoThreadNum = 0) {
	server_.setThreadNum(usrIoThreadNum);
	if (idleAliveTime_ != 0) {
		server_.getLoop()->runEvery(checkIdleTime_,
				boost::bind(&NodeServer::onEveryTime, this));
	}
	server_.start();
	LOG_INFO << "Khala Server is started !enjoy~  :-)";
	loop_.loop();
}
void NodeServer::onEveryTime() {
	//LOG_INFO<<"this is everytime";
	//everytime check overtime conn from tempConnectPool
	tempConnectPool_.checkTempConnect();
}
void NodeServer::onConnection(const TcpConnectionPtr& conn) {
	muduo::Timestamp time = muduo::Timestamp::now();
	if (conn->connected()) {
		//this is connected!
		msgController_.onConnection(conn, time);
	} else {
		//this is disconnected!
		msgController_.onDisConnection(conn, time);
	}
}

void NodeServer::onMessage(const TcpConnectionPtr& conn,
		muduo::net::Buffer* buf, Timestamp time) {
	HeadCode headCode;
	std::string msg = headCode.onCode(conn, buf, time);
	if (!msg.empty()) {
		onDispatcher(conn, msg, time);
	}
}
void NodeServer::onDispatcher(const TcpConnectionPtr& conn, std::string& msg,
		Timestamp time) {
	Json::Value result;
	Json::Reader reader;
	if (reader.parse(msg, result)) {
		std::string typeStr = result[MSG_TYPE].asString();
		msgController_.onDispatcher(conn, result, time, typeStr);
	}
}
bool NodeServer::addNodeType(NodeType* nodeType) {
	if (msgController_.addObjectType(nodeType->getObjectTypeName(), nodeType)) {
		nodeType->setNodeServer(this);
		nodeType->setRegisterMsg_();
		return true;
	}
	LOG_ERROR<<"err nodeType:"<<nodeType->getObjectTypeName()<<" has aready exist! you should set a unique TypeName!";
	return false;
}
void NodeServer::setTempNodeTypeInstance(TempNodeType* tempNodeType) {
	msgController_.setTempNodeType(tempNodeType);
	tempNodeType->setNodeServer(this);
	tempNodeType->setRegisterMsg_();
}
ObjectType* NodeServer::getObjectType(const std::string& type) {
	ObjectType* objectType = 0;
	msgController_.getObjectType(type, &objectType);
	return objectType;
}
