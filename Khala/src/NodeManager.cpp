/*
 * NodeManager.cpp
 *
 *  Created on: Dec 9, 2015
 *      Author: moss
 */
#include <khala/NodeManager.h>
#include <khala/NodeServer.h>
using namespace khala;
bool NodeManager::hasNode(uint id) {
	return nodeServer_->getNodePool()->hasNode(id);
}
bool NodeManager::find(uint id, InfoNodePtr& infoNodePtr) {
	return nodeServer_->getNodePool()->find(id, infoNodePtr);
}
std::vector<uint> NodeManager::getNodeIDs(ObjectType* objectType) {
	return nodeServer_->getNodePool()->getNodeByType(objectType);
}
std::vector<uint> NodeManager::getNodeIDs(std::string& type) {
	ObjectType* objectType = 0;
	nodeServer_->getMsgController().getObjectType(type, &objectType);
	return getNodeIDs(objectType);
}
