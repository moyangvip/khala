/*
 * TempNode.cpp
 *
 *  Created on: Dec 3, 2015
 *      Author: moss
 */

#include <khala/ConnNode.h>
#include <khala/RandomOnlyID.h>
#include <muduo/base/Logging.h>

using namespace khala;
ConnNode::ConnNode(const TcpConnectionPtr& conn,
		Timestamp time) :
		conn_(conn), lastReceiveTime_(time), loginStatus_(NO_LOGIN_STATUS) {
	connID_.setTmpId(RandomOnlyID::getInstance().getOnlyID());
	conn_->setContext((ConnID*) &connID_);
}
ConnNode::~ConnNode() {
	if (loginStatus_ == NO_LOGIN_STATUS) {
		LOG_INFO << conn_->peerAddress().toIpPort() << " temp ID:"
				<< connID_.getTmpId() << " ConnNode will be destroy!";
	} else {
		LOG_INFO << conn_->peerAddress().toIpPort() << " ID:" << connID_.getId()
				<< " temp ID:" << connID_.getTmpId()
				<< " ConnNode will be destroy!";
	}
	conn_->forceClose();
	RandomOnlyID::getInstance().releaseID(getTmpId());
	SET_TMP_ID(conn_, DEFAULT_ID);
	SET_ID(conn_, DEFAULT_ID);
}
TcpConnectionPtr& ConnNode::getTcpConnectionPtr() {
	return conn_;
}
Timestamp ConnNode::getLastReceiveTime() {
	return lastReceiveTime_;
}
void ConnNode::setLastReceiveTime(Timestamp time) {
	this->lastReceiveTime_ = time;
}
uint ConnNode::getTmpId() const {
	return connID_.getTmpId();
}
void ConnNode::setId(uint id) {
	SET_ID(conn_, id);
	connID_.setId(id);
}
uint ConnNode::getId() const {
	return connID_.getId();
}
LoginStatus ConnNode::getStatus() {
	return loginStatus_;
}

void ConnNode::setStatus(LoginStatus loginStatus) {
	loginStatus_ = loginStatus;
}
void ConnNode::send(const std::string& msg) {
	conn_->send(msg);
}
