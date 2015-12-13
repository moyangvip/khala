/*
 * TempNode.h
 *
 *  Created on: Dec 3, 2015
 *      Author: moss
 */

#ifndef COLLECTION_TEMPNODE_H_
#define COLLECTION_TEMPNODE_H_
#include <muduo/net/TcpConnection.h>
#include <boost/shared_ptr.hpp>
#include <khala/ConnID.h>
#include <khala/Keywords.h>
namespace khala {
enum LoginStatus {
	NO_LOGIN_STATUS, LOGIN_STATUS
};
class ConnNode {
public:
	ConnNode(const muduo::net::TcpConnectionPtr& conn, muduo::Timestamp time);
	~ConnNode();
	muduo::net::TcpConnectionPtr& getTcpConnectionPtr() {
		return conn_;
	}
	muduo::Timestamp getLastReceiveTime() {
		return lastReceiveTime_;
	}
	void setLastReceiveTime(muduo::Timestamp time) {
		this->lastReceiveTime_ = time;
	}
	uint getTmpId() const {
		return connID_.getTmpId();
	}
	void setId(uint id) {
		SET_ID(conn_, id);
		connID_.setId(id);
	}
	uint getId() const {
		return connID_.getId();
	}
	LoginStatus getStatus() {
		return loginStatus_;
	}

	void setStatus(LoginStatus loginStatus) {
		loginStatus_ = loginStatus;
	}
	void send(const std::string& msg) {
		conn_->send(msg);
	}
private:
	//careful conn life time
	muduo::net::TcpConnectionPtr conn_;
	//connection time
	muduo::Timestamp lastReceiveTime_;
	ConnID connID_;
	LoginStatus loginStatus_;
};
typedef boost::shared_ptr<ConnNode> ConnNodePtr;
}

#endif /* COLLECTION_TEMPNODE_H_ */
