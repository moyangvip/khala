/*
 * MessageHandler.h
 *
 *  Created on: Dec 3, 2015
 *      Author: moss
 */

#ifndef MSGCONTROLLER_H_
#define MSGCONTROLLER_H_
#include <json/json.h>
#include <muduo/net/TcpServer.h>
#include <boost/unordered_map.hpp>
#include <khala/Keywords.h>
#include <khala/TempNodeType.h>
namespace khala{
class NodeServer;
class MsgController {
public:
	MsgController(NodeServer* nodeServer) :
			nodeServer_(nodeServer) {

	}
	void onDispatcher(const muduo::net::TcpConnectionPtr& conn,
			Json::Value& msg, muduo::Timestamp time, std::string& typeStr);
	void onConnection(const muduo::net::TcpConnectionPtr& conn,
			muduo::Timestamp time);
	void onDisConnection(const muduo::net::TcpConnectionPtr& conn,
			muduo::Timestamp time);
	bool addObjectType(const std::string& objectTypeStr,ObjectType* objectType);
	bool getObjectType(const std::string& objectTypeStr,ObjectType** objectType);
	void setTempNodeType(TempNodeType* tempNodeType){
		objectTypeMap_[TEMP_NODE_TYPE] = tempNodeType;
	}
private:
	bool onBeforeMessage(InfoNodePtr& infoNodePtr,
			Json::Value& msg, muduo::Timestamp time,const std::string& realHandlerType);
	void onErrTypeMessage(InfoNodePtr& infoNodePtr,
			Json::Value& msg, muduo::Timestamp time);
	void onErrRunMessage(const muduo::net::TcpConnectionPtr& conn,
			Json::Value& msg, muduo::Timestamp time);
	bool onBeforeMessage_(InfoNodePtr& infoNodePtr,
			Json::Value& msg, muduo::Timestamp time);
	void onAbnormalExit(const muduo::net::TcpConnectionPtr& conn,
			InfoNodePtr& infoNodePtr, muduo::Timestamp time);
	bool isLogin(const muduo::net::TcpConnectionPtr& conn, Json::Value& msg,
			muduo::Timestamp time);
	InfoNodePtr getConnNode(const muduo::net::TcpConnectionPtr& conn);
	Json::FastWriter fastWriter;
	NodeServer* nodeServer_;
	typedef boost::unordered_map<std::string,ObjectType*> ObjectTypeMap;
	ObjectTypeMap objectTypeMap_;
};
}


#endif /* MSGCONTROLLER_H_ */
