/*
 * NodePool.h
 *
 *  Created on: Dec 3, 2015
 *      Author: moss
 */

#ifndef COLLECTION_NODEPOOL_H_
#define COLLECTION_NODEPOOL_H_
#include <muduo/base/Mutex.h>
#include <boost/unordered_map.hpp>
#include <khala/InfoNode.h>
#include <typeinfo>
#include <vector>
namespace khala {
class ObjectType;
class InfoNodePool {
	/*
	 private:
	 ConnNodePool(){}
	 ConnNodePool(const ConnNodePool&);
	 ConnNodePool& operator = (const ConnNodePool&);*/
public:
	InfoNodePool() {
	}
	bool hasNode(uint id) {
		muduo::MutexLockGuard lock(connNodeMapLock_);
		typename NewMap::iterator it = connNodeMap_.find(id);
		return it != connNodeMap_.end();
	}
	bool find(uint id, InfoNodePtr& infoNodePtr);
	//locked,only one threading...
	bool setNewConnNode(uint id, InfoNodePtr& connNodePtr);
	void remove(uint id) {
		muduo::MutexLockGuard lock(connNodeMapLock_);
		typename NewMap::iterator it = connNodeMap_.find(id);
		if (it == connNodeMap_.end())
			return;
		connNodeMap_.erase(it);
	}
	std::vector<uint> getNodeByType(ObjectType* objectType);
private:
	typedef boost::unordered_map<uint, InfoNodePtr> NewMap;
	NewMap connNodeMap_;
	mutable muduo::MutexLock connNodeMapLock_;
};
}

#endif /* COLLECTION_NODEPOOL_H_ */
