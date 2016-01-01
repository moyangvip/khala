/*
 * TempConnectPool.cpp
 *
 *  Created on: Nov 16, 2015
 *      Author: moss
 */
#include <khala/TempConnectPool.h>
#include <khala/HeadCode.h>
#include <khala/ParseKey.h>
#include <muduo/base/Timestamp.h>
#include <muduo/base/Logging.h>
#include <json/json.h>
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

			//FIXME:this should be improved
			Json::Value res;
			res[MSG_TYPE] = OVER_TIME;
			res[DATA] = "err,connect overtime!";
			Json::FastWriter jwriter;
			std::string sendStr = jwriter.write(res);
			muduo::net::Buffer buff = HeadCode::getInstance()->addHeadCode(
					sendStr);
			p->getTcpConnectionPtr()->send(&buff);

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
