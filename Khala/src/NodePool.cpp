/*
 * NodePool.cpp
 *
 *  Created on: 2016年1月3日
 *      Author: moss
 */

#include <khala/NodePool.h>
using namespace khala;

NodePool::NodePool() :
		setIdleTime_(true) {
	aliveManager_.setOverTimeCallBack(
			boost::bind(&NodePool::overTimeNode, this, _1));
	if (DEFAULT_IDLE_ALIVE_TIME == 0) {
		//turn off overtime check
		setIdleTime_ = false;
	}
}

NodePool::~NodePool() {
}
bool NodePool::hasNode(uint id) {
	muduo::MutexLockGuard lock(nodeMapLock_);
	typename NewMap::iterator it = nodeMap_.find(id);
	return it != nodeMap_.end();
}
bool NodePool::find(uint id, InfoNodePtr& infoNodePtr) {
	muduo::MutexLockGuard lock(nodeMapLock_);
	typename NewMap::iterator it = nodeMap_.find(id);
	if (it == nodeMap_.end())
		return false;
	infoNodePtr = it->second;
	return true;
}
//locked,only one threading...
bool NodePool::setNewNode(uint id, InfoNodePtr& connNodePtr) {
	//if has old id,don't do anything and return false
	{
		muduo::MutexLockGuard lock(nodeMapLock_);
		typename NewMap::iterator it = nodeMap_.find(id);
		if (it != nodeMap_.end())
			return false;
		nodeMap_[id] = connNodePtr;
	}
	if (setIdleTime_) {
		//try to add to alive
		aliveManager_.push_front(id);
	}
	return true;
}
void NodePool::remove(uint id) {
	{
		muduo::MutexLockGuard lock(nodeMapLock_);
		typename NewMap::iterator it = nodeMap_.find(id);
		if (it == nodeMap_.end())
			return;
		nodeMap_.erase(it);
	}
	if (setIdleTime_) {
		//try to remove from alive
		aliveManager_.remove(id);
	}
}
std::vector<uint> NodePool::getNodeByType(const std::string& type) {
	std::vector<uint> result;
	muduo::MutexLockGuard lock(nodeMapLock_);
	typename NewMap::iterator it = nodeMap_.begin();
	while (it != nodeMap_.end()) {
		if (it->second->getNodeType() == type) {
			result.push_back(it->first);
		}
		++it;
	}
	return result;
}

void NodePool::updateAliveId(uint id) {
	aliveManager_.updateAliveId(id);
}
void NodePool::checkAlive() {
	aliveManager_.checkAlive();
}
void NodePool::setIdleAliveTime(int idleAliveTime) {
	if (idleAliveTime == 0) {
		//turn off overtime check
		setIdleTime_ = false;
	} else {
		setIdleTime_ = true;
	}
	aliveManager_.setIdleAliveTime(idleAliveTime);
}
int NodePool::getIdleAliveTime() {
	return aliveManager_.getIdleAliveTime();
}
void NodePool::setOverTimeCallBack(const OverTimeCallback2& cb) {
	cb_ = cb;
}
void NodePool::overTimeNode(uint id) {
	InfoNodePtr infoNodePtr;
	if (find(id, infoNodePtr)) {
		//invoke NodePool's callback func
		cb_(infoNodePtr, Timestamp::now());
	}
}
