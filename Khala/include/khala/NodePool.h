/*
 * NodePool.h
 *
 *  Created on: 2016年1月3日
 *      Author: moss
 */

#ifndef SRC_NODEPOOL_H_
#define SRC_NODEPOOL_H_
#include <khala/AliveManager.h>
#include <khala/InfoNode.h>
#include <khala/MsgController.h>
#include <vector>
#include <boost/unordered_map.hpp>
namespace khala {
typedef boost::function<void(InfoNodePtr&, Timestamp time)> OverTimeCallback2;
class NodePool {
public:
	NodePool();
	virtual ~NodePool();
	bool hasNode(uint id);
	bool find(uint id, InfoNodePtr& infoNodePtr);
	//locked,only one threading...
	bool setNewNode(uint id, InfoNodePtr& connNodePtr);
	void remove(uint id);

	//this is about nodeManager
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

	//this is about aliveTime
	//change id to map head
	void updateAliveId(uint id);
	void checkAlive();
	/*
	 * you can set node idle time,if idleAliveTime = 0,turn off over time check
	 * */
	void setIdleAliveTime(int idleAliveTime);
	int getIdleAliveTime();
	void setOverTimeCallBack(const OverTimeCallback2& cb);
private:
	//this is AliveManager's overTime callback func
	void overTimeNode(uint id);
private:
	typedef boost::unordered_map<uint, InfoNodePtr> NewMap;
	NewMap nodeMap_;
	mutable muduo::MutexLock nodeMapLock_;
	AliveManager aliveManager_;
	OverTimeCallback2 cb_;
	//is set over time check?
	bool setIdleTime_;
};
template<class Type>
std::vector<uint> NodePool::getAllNodeByType(Type* objectType,
		MsgController* msgController) {
	muduo::MutexLockGuard lock(nodeMapLock_);
	std::vector<uint> result;
	typename NewMap::iterator it = nodeMap_.begin();
	while (it != nodeMap_.end()) {
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
} /* namespace khala */

#endif /* SRC_NODEPOOL_H_ */
