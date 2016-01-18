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
	bool hasNode(uint id,const std::string& type);
	bool find(uint id, InfoNodePtr& infoNodePtr);
	bool find(uint id, InfoNodePtr& infoNodePtr,const std::string& type);
	//locked,only one threading...
	bool setNewNode(uint id, InfoNodePtr& infoNodePtr);
	bool remove(uint id);
	bool remove(uint id,const std::string& type);
	/*
	 * try to remove use type,if false,check from all type
	 * */
	void forceRemove(uint id, const std::string& type);
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
	bool hasNodeUnlock(uint id);
private:
	mutable muduo::MutexLock nodeMapLock_;
	AliveManager aliveManager_;
	OverTimeCallback2 cb_;
	//is set over time check?
	bool setIdleTime_;

	typedef boost::unordered_map<uint, InfoNodePtr> NewIDMap;
	typedef boost::shared_ptr<NewIDMap> NewIDMapPtr;
	typedef boost::unordered_map<std::string, NewIDMapPtr> NewTypeMap;
	NewTypeMap nodeCache_;

};
template<class Type>
std::vector<uint> NodePool::getAllNodeByType(Type* objectType,
		MsgController* msgController) {
	std::vector<uint> result;
	muduo::MutexLockGuard lock(nodeMapLock_);
	typename NewTypeMap::iterator it1 = nodeCache_.begin();
	for (; it1 != nodeCache_.end(); ++it1) {
		ObjectType* myObjectType = 0;
		msgController->getObjectType(it1->first, &myObjectType);
		Type* p = dynamic_cast<Type*>(myObjectType);
		if (p != 0) {
			//add all the node id of the type
			typename NewIDMap::iterator it2 = (it1->second)->begin();
			for (; it2 != (it1->second)->end(); ++it2) {
				result.push_back(it2->first);
			}
		}
	}
	return result;
}
} /* namespace khala */

#endif /* SRC_NODEPOOL_H_ */
