/*
 * NodeType.cpp
 *
 *  Created on: Dec 8, 2015
 *      Author: moss
 */
#include <khala/NodeServer.h>
#include <khala/NodeType.h>
#include <khala/ParseKey.h>
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
	nodeServer_->getNodePool()->remove(id);
	infoNodePtr->setStatus(NO_LOGIN_STATUS);
	LOG_INFO << conn->peerAddress().toIpPort() << " ID:" << ID_CONN(conn)
			<< " logout success!";
	return true;
}


bool NodeType::onNodeIdMsg(InfoNodePtr& infoNodePtr, Json::Value& msg,
			Timestamp time){
	uint id = infoNodePtr->getId();
	std::stringstream ss;
	ss<<"Node id:"<<id;
	infoNodePtr->send(ss.str());
	return true;
}
