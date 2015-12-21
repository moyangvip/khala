/*
 * TempConnectPool.h
 *
 *  Created on: Nov 16, 2015
 *      Author: moss
 */

#ifndef TEMPCONNECTPOOL_H_
#define TEMPCONNECTPOOL_H_

#include <khala/ConnNode.h>
#include <khala/LinkedHashSet.h>
#include <khala/Config.h>
namespace khala {
//users connected but not login
class TempConnectPool {
public:
	TempConnectPool(int idleAliveTime= DEFAULT_IDLE_ALIVE_TIME);
	void checkTempConnect();
	void setIdleAliveTime(int idleAliveTime);
	bool push_front(ConnNodePtr& tempNodePtr);

	bool find(uint id, ConnNodePtr& tempNodePtr);
	void remove(uint id);
	//change conn to temp pool head
	void updateTempConnect(uint id);
private:
	moss::LinkedHashMap<uint, ConnNodePtr> tmpMap_;
	mutable muduo::MutexLock tmpMapLock_;
	int idleAliveTime_;
};
}

#endif /* TEMPCONNECTPOOL_H_ */
