/*
 * NodePool.cpp
 *
 *  Created on: 2016年1月3日
 *      Author: moss
 */

#include <khala/NodePool.h>
#include <muduo/base/Logging.h>
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
	return hasNodeUnlock(id);
}
bool NodePool::hasNode(uint id, const std::string& type) {
	muduo::MutexLockGuard lock(nodeMapLock_);
	typename NewTypeMap::iterator it1 = nodeCache_.find(type);
	if (it1 == nodeCache_.end()) {
		return false;
	}
	typename NewIDMap::iterator it2 = (it1->second)->find(id);
	if (it2 != (it1->second)->end()) {
		return true;
	}
	return false;
}
bool NodePool::hasNodeUnlock(uint id) {
	typename NewTypeMap::iterator it1 = nodeCache_.begin();
	for (; it1 != nodeCache_.end(); ++it1) {
		typename NewIDMap::iterator it2 = (it1->second)->find(id);
		if (it2 != (it1->second)->end()) {
			return true;
		}
	}
	return false;
}
bool NodePool::find(uint id, InfoNodePtr& infoNodePtr) {
	muduo::MutexLockGuard lock(nodeMapLock_);
	typename NewTypeMap::iterator it1 = nodeCache_.begin();
	for (; it1 != nodeCache_.end(); ++it1) {
		typename NewIDMap::iterator it2 = (it1->second)->find(id);
		if (it2 != (it1->second)->end()) {
			infoNodePtr = it2->second;
			return true;
		}
	}
	return false;
}
bool NodePool::find(uint id, InfoNodePtr& infoNodePtr,
		const std::string& type) {
	muduo::MutexLockGuard lock(nodeMapLock_);
	typename NewTypeMap::iterator it1 = nodeCache_.find(type);
	if (it1 == nodeCache_.end()) {
		return false;
	}
	typename NewIDMap::iterator it2 = (it1->second)->find(id);
	if (it2 != (it1->second)->end()) {
		infoNodePtr = it2->second;
		return true;
	}
	return false;
}
//locked,only one threading...
bool NodePool::setNewNode(uint id, InfoNodePtr& infoNodePtr) {
	{
		muduo::MutexLockGuard lock(nodeMapLock_);
		//if has old id,don't do anything and return false
		if (hasNodeUnlock(id))
			return false;
		std::string type = infoNodePtr->getNodeType();
		if (nodeCache_.find(type) == nodeCache_.end()) {
			NewIDMapPtr newIDMapPtr(new NewIDMap);
			nodeCache_[type] = newIDMapPtr;
			//LOG_INFO << "add new  type node Pool:" << type << " Type!";
		}
		(*(nodeCache_[type]))[id] = infoNodePtr;
	} //this is end of nodeMapLock_
	if (setIdleTime_) {
		//try to add to alive
		aliveManager_.push_front(id);
	}
	return true;
}
bool NodePool::remove(uint id, const std::string& type) {
	bool res = false;
	{
		muduo::MutexLockGuard lock(nodeMapLock_);
		typename NewTypeMap::iterator it1 = nodeCache_.find(type);
		if (it1 == nodeCache_.end()) {
			return false;
		}
		typename NewIDMap::iterator it2 = (it1->second)->find(id);
		if (it2 != (it1->second)->end()) {
			res = true;
			//delete the node
			(it1->second)->erase(it2);
			if ((it1->second)->empty()) {
				//if the NewIDMapPtr is empty,delete it
				//be careful we will delete the iterator,we will break the loop,so it is safe
				nodeCache_.erase(it1);
				//LOG_INFO << type << " node Pool is empty! is deleted!";
			}
		}
	}//this is end of nodeMapLock_
	if (setIdleTime_ && res == true) {
		//try to remove from alive
		aliveManager_.remove(id);
	}
	return res;
}
void NodePool::forceRemove(uint id, const std::string& type) {
	//try to remove from this type nodePool,first try effective way
	if (remove(id, type)) {
		return;
	}
	LOG_ERROR<< " ID:" << id<< " can't delete from node Pool as "<< type << " Type!";
	//if false,try normal way
	remove(id);
}
bool NodePool::remove(uint id) {
	bool res = false;
	{
		muduo::MutexLockGuard lock(nodeMapLock_);
		typename NewTypeMap::iterator it1 = nodeCache_.begin();
		for (; it1 != nodeCache_.end(); ++it1) {
			typename NewIDMap::iterator it2 = (it1->second)->find(id);
			if (it2 != (it1->second)->end()) {
				res = true;
				//delete the node
				(it1->second)->erase(it2);
				if ((it1->second)->empty()) {
					std::string type = it1->first;
					//if the NewIDMapPtr is empty,delete it
					//be careful we will delete the iterator,we will break the loop,so it is safe
					nodeCache_.erase(it1);
					//LOG_INFO << type << " node Pool is empty! is deleted!";
				}
				//we have delete,break the loop
				break;
			}
		}
	} //this is end of nodeMapLock_
	if (setIdleTime_ && res == true) {
		//try to remove from alive
		aliveManager_.remove(id);
	}
	return res;
}
std::vector<uint> NodePool::getNodeByType(const std::string& type) {
	std::vector<uint> result;
	muduo::MutexLockGuard lock(nodeMapLock_);
	typename NewTypeMap::iterator it1 = nodeCache_.find(type);
	if (it1 != nodeCache_.end()) {
		typename NewIDMap::iterator it2 = (it1->second)->begin();
		for (; it2 != (it1->second)->end(); ++it2) {
			result.push_back(it2->first);
		}
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
