/*
 * NodeServer.h
 *
 *  Created on: Dec 3, 2015
 *      Author: moss
 */

#ifndef NODESERVER_H_
#define NODESERVER_H_

#include <json/json.h>
#include <khala/InfoNodePool.h>
#include <khala/MsgController.h>
#include <khala/NodeType.h>
#include <khala/TempConnectPool.h>
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
namespace khala {
typedef muduo::net::InetAddress InetAddress;
class RegisterMsg;
class NodeServer {
public:
	NodeServer(const InetAddress& listenAddr);
	virtual ~NodeServer();
	//never return
	void start(int usrIoThreadNum);
	//if seconds = 0,turn off,must be used before start
	void setIdleTime(int seconds) {
		idleAliveTime_ = seconds;
		this->getTempConnectPool()->setIdleAliveTime(idleAliveTime_);
	}
	/*
	 * your must set a unique nodeType to NodeType
	 * like NodeType.getObjectType() get "node_type" as the unique nodeType key
	 * */
	bool addNodeType(NodeType* nodeType);
	InfoNodePool* getNodePool() {
		return &nodePool_;
	}
	TempConnectPool* getTempConnectPool() {
		return &tempConnectPool_;
	}
	MsgController& getMsgController() {
		return msgController_;
	}
	void setTempNodeTypeInstance(TempNodeType* tempNodeType) ;
	ObjectType* getObjectType(const std::string& type) ;
private:
	void onConnection(const TcpConnectionPtr& conn);
	void onMessage(const TcpConnectionPtr& conn,
			muduo::net::Buffer* buf, Timestamp time);
	void onDispatcher(const TcpConnectionPtr& conn,
			std::string& msg, Timestamp time);
	//void registerMsgHandler();
	//every time to process idle conn
	void onEveryTime();
	//muduo core
	muduo::net::EventLoop loop_;
	//muduo core
	muduo::net::TcpServer server_;
	//default 60s.after idleTime_,idle connect will turn off if not login
	//if idleTime_ = 0,never turn off idle connect
	//idleAliveTime_ = (idleAliveTime_> checkIdleTime_ ?idleAliveTime_:checkIdleTime_)....exclude 0
	int idleAliveTime_;
	//default 10s, every checkIdleTime will check curr each tmp conn
	int checkIdleTime_;
	TempConnectPool tempConnectPool_;
	InfoNodePool nodePool_;
	MsgController msgController_;
};
}

#endif /* NODESERVER_H_ */
