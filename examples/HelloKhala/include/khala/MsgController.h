/*
 * MessageHandler.h
 *
 *  Created on: Dec 3, 2015
 *      Author: moss
 */

#ifndef MSGCONTROLLER_H_
#define MSGCONTROLLER_H_
#include <khala/InfoNode.h>
#include <json/json.h>
#include <boost/unordered_map.hpp>
namespace khala{
class NodeServer;
class TempNodeType;
class ObjectType;
class MsgController {
public:
	MsgController(NodeServer* nodeServer) :
			nodeServer_(nodeServer) {

	}
	void onDispatcher(const TcpConnectionPtr& conn,
			Json::Value& msg,Timestamp time, std::string& typeStr);
	void onConnection(const TcpConnectionPtr& conn,
			Timestamp time);
	void onDisConnection(const TcpConnectionPtr& conn,
			Timestamp time);
	bool addObjectType(const std::string& objectTypeStr,ObjectType* objectType);
	bool getObjectType(const std::string& objectTypeStr,ObjectType** objectType);
	void setTempNodeType(TempNodeType* tempNodeType);
	void onOverTime(InfoNodePtr& infoNodePtr,Timestamp time);
private:
	bool onBeforeMessage(InfoNodePtr& infoNodePtr,
			Json::Value& msg, Timestamp time,const std::string& realHandlerType);
	void onErrTypeMessage(InfoNodePtr& infoNodePtr,
			Json::Value& msg,Timestamp time);
	void onErrRunMessage(const TcpConnectionPtr& conn,
			Json::Value& msg, Timestamp time);
	bool onBeforeMessage_(InfoNodePtr& infoNodePtr,
			Json::Value& msg, Timestamp time);
	void onAbnormalExit(const TcpConnectionPtr& conn,
			InfoNodePtr& infoNodePtr, Timestamp time);
	InfoNodePtr getInfoNode(const TcpConnectionPtr& conn);
	void updateAliveTime(InfoNodePtr& infoNodePtr);
private:
	Json::FastWriter fastWriter;
	NodeServer* nodeServer_;
	typedef boost::unordered_map<std::string,ObjectType*> ObjectTypeMap;
	ObjectTypeMap objectTypeMap_;
};
}


#endif /* MSGCONTROLLER_H_ */
