/*
 * TempConnectPool.cpp
 *
 *  Created on: Nov 16, 2015
 *      Author: moss
 */
#include <khala/TempConnectPool.h>
#include <muduo/base/Timestamp.h>
#include <muduo/base/Logging.h>
using namespace khala;
TempConnectPool::TempConnectPool(int idleAliveTime) :
		idleAliveTime_(idleAliveTime) {
}
void TempConnectPool::setIdleAliveTime(int idleAliveTime) {
	idleAliveTime_ = idleAliveTime;
}
bool TempConnectPool::push_front(ConnNodePtr& tempNodePtr) {
	uint id = tempNodePtr->getTmpId();
	return tmpMap_.push_front(id, tempNodePtr);
}
void TempConnectPool::remove(uint id) {
	//must be lock,otherwise may create null point
	muduo::MutexLockGuard lock(tmpMapLock_);
	tmpMap_.remove(id);
}
void TempConnectPool::checkTempConnect() {
	muduo::MutexLockGuard lock(tmpMapLock_);
	while (tmpMap_.size() > 0) {
		ConnNodePtr p = tmpMap_.back();
		if (p == 0)
			break;
		if (addTime(p->getLastReceiveTime(), idleAliveTime_)
				< muduo::Timestamp::now()) {
			LOG_INFO << p->getTcpConnectionPtr()->peerAddress().toIpPort()
					<< " temp ID:" << p->getTmpId()
					<< " is OVERTIME,will be disconnect!";
			//FIXME:may be force close when sending...
			p->getTcpConnectionPtr()->send("err,connect overtime!");
			tmpMap_.pop_back();
		} else {
			//the last don't overtime,so all is ok
			break;
		}
	}
}
void TempConnectPool::updateTempConnect(uint id) {
	muduo::MutexLockGuard lock(tmpMapLock_);
	ConnNodePtr* p = tmpMap_.find(id);
	if (p == 0)
		return;
	(*p)->setLastReceiveTime(muduo::Timestamp::now());
	tmpMap_.updateKey(id);
}
bool TempConnectPool::find(uint id, ConnNodePtr& tempNodePtr) {
	muduo::MutexLockGuard lock(tmpMapLock_);
	ConnNodePtr* p = tmpMap_.find(id);
	if (p == 0)
		return false;
	tempNodePtr = *p;
	return true;
}
