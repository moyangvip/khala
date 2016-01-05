/*
 * NodeServer.h
 *
 *  Created on: Dec 3, 2015
 *      Author: moss
 */

#ifndef NODESERVER_H_
#define NODESERVER_H_

#include <khala/MsgController.h>
#include <khala/NodePool.h>
#include <muduo/net/EventLoop.h>
namespace khala {
class RegisterMsg;
class NodeType;
class NodePool;
class NodeServer {
public:
	NodeServer(const InetAddress& listenAddr);
	virtual ~NodeServer();
	//never return
	void start(int usrIoThreadNum);
	/*
	 * temp node max alive time if not get valid msg
	 * if seconds = 0,turn off  TempNode over time check
	 * must be used before start
	 * */
	void setTempNodeIdleTime(int seconds);
	/*
	 * node max alive time if not get valid msg
	 * if seconds = 0,turn off login node over time check
	 * must be used before start
	 * */
	void setNodeIdleTime(int seconds);
	/*
	 * it will check idle node periodically,must less than idleTime
	 * if seconds=0,set TempNodeIdleTime and NodeIdleTime = 0,and turn off all node over time check
	 * */
	void setCheckTime(int seconds);
	/*
	 * your can set a new NodeType
	 * */
	bool addNodeType(NodeType* nodeType);
	/*
	 * you can set a new TempNodeType
	 * */
	void setTempNodeType(TempNodeType* tempNodeType);



	ObjectType* getObjectType(const std::string& type);
	NodePool* getNodePool() {
		return &nodePool_;
	}
	NodePool* getTempNodePool() {
		return &tempNodePool_;
	}
	MsgController& getMsgController() {
		return msgController_;
	}
private:
	void onConnection(const TcpConnectionPtr& conn);
	void onMessage(const TcpConnectionPtr& conn, muduo::net::Buffer* buf,
			Timestamp time);
	void onDispatcher(const TcpConnectionPtr& conn, std::string& msg,
			Timestamp time);
	//every time to process idle conn
	void onEveryTime();

private:
	//muduo core
	muduo::net::EventLoop loop_;
	//muduo core
	muduo::net::TcpServer server_;
	//default 10s, every checkIdleTime will check curr each  conn
	int checkIdleTime_;
	NodePool tempNodePool_;
	NodePool nodePool_;
	MsgController msgController_;
};
}

#endif /* NODESERVER_H_ */
