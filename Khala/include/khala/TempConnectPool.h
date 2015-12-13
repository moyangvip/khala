/*
 * TempConnectPool.h
 *
 *  Created on: Nov 16, 2015
 *      Author: moss
 */

#ifndef TEMPCONNECTPOOL_H_
#define TEMPCONNECTPOOL_H_
#include <khala/Config.h>
#include <khala/ConnNode.h>
#include <khala/Keywords.h>
#include <khala/LinkedHashSet.h>
#include <muduo/base/Logging.h>
namespace khala {
//users connected but not login
class TempConnectPool {
public:
	TempConnectPool(int idleAliveTime = DEFAULT_IDLE_ALIVE_TIME) :
			idleAliveTime_(idleAliveTime) {
	}
	void checkTempConnect();
	void setIdleAliveTime(int idleAliveTime) {
		idleAliveTime_ = idleAliveTime;
	}
	bool push_front(ConnNodePtr& tempNodePtr) {
		uint id = tempNodePtr->getTmpId();
		return tmpMap_.push_front(id, tempNodePtr);
	}

	bool find(uint id, ConnNodePtr& tempNodePtr);
	void remove(uint id) {
		//must be lock,otherwise may create null point
		muduo::MutexLockGuard lock(tmpMapLock_);
		tmpMap_.remove(id);
	}
	//change conn to temp pool head
	void updateTempConnect(uint id);
private:
	moss::LinkedHashMap<uint, ConnNodePtr> tmpMap_;
	mutable muduo::MutexLock tmpMapLock_;
	int idleAliveTime_;
};
}

#endif /* TEMPCONNECTPOOL_H_ */
