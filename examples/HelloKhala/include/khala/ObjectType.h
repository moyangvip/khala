/*
 * ObjectType.h
 *
 *  Created on: Dec 8, 2015
 *      Author: moss
 */

#ifndef OBJECTTYPE_H_
#define OBJECTTYPE_H_
#include <khala/InfoNode.h>
#include <khala/NodeManager.h>
#include <json/json.h>
#include <khala/ParseKey.h>
#include <muduo/net/TcpServer.h>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/unordered_map.hpp>

namespace khala {
typedef boost::function<bool(InfoNodePtr&, Json::Value&, Timestamp)> RegisterMessageCallback;
typedef boost::unordered_map<std::string, RegisterMessageCallback> MsgHandlerMap;
class NodeServer;
class RegisterHandler {
public:
	RegisterHandler(MsgHandlerMap& msgHandlerMap);
	bool setRegisterMsg(const std::string & type,
			const RegisterMessageCallback& cb);
private:
	MsgHandlerMap& msgHandlerMap_;
};

class ObjectType {
public:
	ObjectType();
	virtual ~ObjectType();
	static ObjectType* getInstance();
	virtual void setRegisterMsg(RegisterHandler& handler)=0;
	virtual const std::string& getObjectTypeName()=0;
	virtual MsgHandlerMap& getRegisterHandler();
	/* TODO:
	 * if receive any type msg should be process after logining,will invoke this function
	 *
	 virtual void onNoLoginMessage(InfoNodePtr& infoNodePtr, Json::Value& msg,
	 muduo::Timestamp time)=0;
	 */
	/*
	 * if receive any no register type msg with this nodeType,will invoke this function
	 * */
	virtual void onErrTypeMessage(InfoNodePtr& infoNodePtr, Json::Value& msg,
			Timestamp time) ;
	/*
	 * if  any msg handler return false,will invoke this function
	 * */
	virtual void onErrRunMessage(InfoNodePtr& infoNodePtr, Json::Value& msg,
			Timestamp time);
	/*
	 * if node overTime,will invoke this function
	 * the node connection will be disconnect soon
	 * */
	virtual void onOverTime(InfoNodePtr& infoNodePtr,Timestamp time);
public:
	friend class NodeServer;
	friend class NodeType;
	friend class TempNodeType;
	friend class  MsgController;
protected:
	/*
	 * you can get NodeManager to manager node
	 * */
	NodeManager* getNodeManager();
	/*
	 * you can get ObectType Object from objectType string
	 * */
	ObjectType* getObjectType(const std::string& objectType);
private:
	void setRegisterMsg_();
	void setNodeServer(NodeServer* nodeServer);
	void onOverTime_(InfoNodePtr& infoNodePtr, Timestamp time);
private:
	MsgHandlerMap msgHandlerMap_;
	NodeManager nodeManager_;
	NodeServer* nodeServer_;
};
}
#endif /* OBJECTTYPE_H_ */
