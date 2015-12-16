/*
 * NodePool.cpp
 *
 *  Created on: Dec 3, 2015
 *      Author: moss
 */
#include <khala/InfoNodePool.h>
using namespace khala;
bool InfoNodePool::find(uint id, InfoNodePtr& infoNodePtr) {
	muduo::MutexLockGuard lock(connNodeMapLock_);
	typename NewMap::iterator it = connNodeMap_.find(id);
	if (it == connNodeMap_.end())
		return false;
	infoNodePtr = it->second;
	return true;
}
//locked,only one threading...
bool InfoNodePool::setNewConnNode(uint id, InfoNodePtr& infoNodePtr) {
	//if has old id,don't do anything and return false
	muduo::MutexLockGuard lock(connNodeMapLock_);
	typename NewMap::iterator it = connNodeMap_.find(id);
	if (it != connNodeMap_.end())
		return false;
	connNodeMap_[id] = infoNodePtr;
	return true;
}
std::vector<uint> InfoNodePool::getNodeByType(ObjectType* objectType){
		std::vector<uint> result;
		muduo::MutexLockGuard lock(connNodeMapLock_);
		typename NewMap::iterator it = connNodeMap_.begin();
		while(it != connNodeMap_.end()){
			if(typeid(it->second->getNodeType()) == typeid(objectType)){
				result.push_back(it->first);
			}
			++it;
		}
		return result;
	}
bool InfoNodePool::hasNode(uint id) {
	muduo::MutexLockGuard lock(connNodeMapLock_);
	typename NewMap::iterator it = connNodeMap_.find(id);
	return it != connNodeMap_.end();
}
void InfoNodePool::remove(uint id) {
		muduo::MutexLockGuard lock(connNodeMapLock_);
		typename NewMap::iterator it = connNodeMap_.find(id);
		if (it == connNodeMap_.end())
			return;
		connNodeMap_.erase(it);
	}
