/*
 * NodeManager.h
 *
 *  Created on: Dec 9, 2015
 *      Author: moss
 */

#ifndef NODEMANAGER_H_
#define NODEMANAGER_H_
#include <khala/InfoNode.h>
#include <khala/Keywords.h>
#include <khala/NodeServer.h>
#include <vector>
namespace khala {
class NodeManager {
public:
	NodeManager() :
			nodeServer_(0) {
	}
	bool hasNode(uint id);
	bool find(uint id, InfoNodePtr& infoNodePtr);
	/*
	 * get node ids which only have  the same  class
	 * */
	std::vector<uint> getNodeIDs(ObjectType* objectType);
	std::vector<uint> getNodeIDs(const std::string& type);
	/*
	 * get node ids which have the same  class and derived class
	 * */
	template<class Type>
	std::vector<uint> getAllNodeIDs(Type objectType);
	template<class Type>
	std::vector<uint> getAllNodeIDs(Type* objectType);
	//std::vector<uint> getAllNodeIDs(const std::string& type);
	friend class ObjectType;
private:
	void setNodeServer(NodeServer* nodeServer) {
		nodeServer_ = nodeServer;
	}
	NodeServer* nodeServer_;
};
template<class Type>
std::vector<uint> NodeManager::getAllNodeIDs(Type objectType) {
	ObjectType* p = dynamic_cast<ObjectType*>(&objectType);
	if(p != 0){
		return nodeServer_->getNodePool()->getAllNodeByType(&objectType,&(nodeServer_->getMsgController()));
	}
	std::vector<uint> v;
	return v;
}
template<class Type>
	std::vector<uint> NodeManager::getAllNodeIDs(Type* objectType){
	ObjectType* p = dynamic_cast<ObjectType*>(objectType);
		if(p != 0){
			return nodeServer_->getNodePool()->getAllNodeByType(objectType,&(nodeServer_->getMsgController()));
		}
		std::vector<uint> v;
		return v;
}
}

#endif /* NODEMANAGER_H_ */
