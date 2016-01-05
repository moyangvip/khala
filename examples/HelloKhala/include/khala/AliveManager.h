/*
 * AliveManager.h
 *
 *  Created on: 2016年1月2日
 *      Author: moss
 */

#ifndef SRC_ALIVEMANAGER_H_
#define SRC_ALIVEMANAGER_H_
#include <khala/LinkedHashSet.h>
#include <khala/Config.h>
#include <khala/Keywords.h>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
namespace khala {
class AliveID {
public:
	AliveID(uint id,Timestamp time):id_(id),time_(time){}
	uint id_;
	Timestamp time_;
};
typedef boost::shared_ptr<AliveID> AliveIDPtr;
typedef boost::function<void(uint)> OverTimeCallback1;
class AliveManager {
public:
	AliveManager(int idleAliveTime = DEFAULT_IDLE_ALIVE_TIME);
	virtual ~AliveManager();
	void setOverTimeCallBack(const OverTimeCallback1& cb);
	void setIdleAliveTime(int idleAliveTime);
	int getIdleAliveTime();
	void checkAlive();
	bool push_front(uint id);
	bool find(uint id);
	void remove(uint id);
	//change id to map head
	void updateAliveId(uint id);
private:
	moss::LinkedHashMap<uint, AliveIDPtr> aliveIdMap_;
	mutable muduo::MutexLock aliveLock_;
	OverTimeCallback1 cb_;
	/*
	 * if idleTime_ = 0,never turn off idle connect
	 * idleAliveTime_ = (idleAliveTime_> checkIdleTime_ ?idleAliveTime_:checkIdleTime_)....exclude 0
	 * */
	int idleAliveTime_;
};
}

#endif /* SRC_ALIVEMANAGER_H_ */
