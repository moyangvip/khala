/*
 * AliveManager.cpp
 *
 *  Created on: 2016年1月2日
 *      Author: moss
 */

#include <khala/AliveManager.h>
using namespace khala;
AliveManager::AliveManager(int idleAliveTime) :
		idleAliveTime_(idleAliveTime) {

}

AliveManager::~AliveManager() {
}
void AliveManager::setOverTimeCallBack(const OverTimeCallback1& cb){
	cb_ = cb;
}
void AliveManager::setIdleAliveTime(int idleAliveTime){
	idleAliveTime_ = idleAliveTime;
}
int AliveManager::getIdleAliveTime(){
	return idleAliveTime_;
}
bool AliveManager::push_front(uint id) {
	muduo::MutexLockGuard lock(aliveLock_);
	AliveIDPtr aliveIDPtr(new AliveID(id, Timestamp::now()));
	return aliveIdMap_.push_front(id, aliveIDPtr);
}
bool AliveManager::find(uint id) {
	muduo::MutexLockGuard lock(aliveLock_);
	AliveIDPtr* p = aliveIdMap_.find(id);
	if (p == 0)
		return false;
	return true;
}
void AliveManager::remove(uint id) {
	//must be lock,otherwise may create null point
	muduo::MutexLockGuard lock(aliveLock_);
	aliveIdMap_.remove(id);
}
//change id to map head
void AliveManager::updateAliveId(uint id) {
	muduo::MutexLockGuard lock(aliveLock_);
	AliveIDPtr* p = aliveIdMap_.find(id);
	if (p == 0)
		return;
	(*p)->time_ = Timestamp::now();
	aliveIdMap_.updateKey(id);
}
void AliveManager::checkAlive() {
	muduo::MutexLockGuard lock(aliveLock_);
	if(idleAliveTime_ ==  0){
		return ;
	}
	while (aliveIdMap_.size() > 0) {
		AliveIDPtr p = aliveIdMap_.back();
		if (p == 0)
			break;
		if (addTime(p->time_, idleAliveTime_) < muduo::Timestamp::now()) {
			//overtime callback
			cb_(p->id_);
			aliveIdMap_.pop_back();
		} else {
			//the last don't overtime,so all is ok
			break;
		}
	}
}
