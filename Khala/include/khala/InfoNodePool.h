/*
 * NodePool.h
 *
 *  Created on: Dec 3, 2015
 *      Author: moss
 */

#ifndef COLLECTION_NODEPOOL_H_
#define COLLECTION_NODEPOOL_H_
#include <khala/InfoNode.h>
#include <khala/MsgController.h>
#include <muduo/base/Mutex.h>
#include <boost/unordered_map.hpp>
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
	bool hasNode(uint id);
	bool find(uint id, InfoNodePtr& infoNodePtr);
	//locked,only one threading...
	bool setNewConnNode(uint id, InfoNodePtr& connNodePtr);
	void remove(uint id);
	/*
	 * get node ids which have the same  class and derived class
	 * */
	template<class Type>
	std::vector<uint> getAllNodeByType(Type* objectType,
			MsgController* msgController);
	/*
	 * get node ids which only have  the same  class
	 * */
	std::vector<uint> getNodeByType(const std::string& type);
private:
	typedef boost::unordered_map<uint, InfoNodePtr> NewMap;
	NewMap connNodeMap_;
	mutable muduo::MutexLock connNodeMapLock_;
};
template<class Type>
std::vector<uint> InfoNodePool::getAllNodeByType(Type* objectType,
		MsgController* msgController) {
	std::vector<uint> result;
	muduo::MutexLockGuard lock(connNodeMapLock_);
	typename NewMap::iterator it = connNodeMap_.begin();
	while (it != connNodeMap_.end()) {
		ObjectType* myObjectType = 0;
		msgController->getObjectType(it->second->getNodeType(), &myObjectType);
		Type* p = dynamic_cast<Type*>(myObjectType);
		if (p != 0) {
			result.push_back(it->first);
		}
		++it;
	}
	return result;
}
}

#endif /* COLLECTION_NODEPOOL_H_ */
