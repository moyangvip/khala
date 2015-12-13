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
#include <vector>
namespace khala {
class NodeServer;
class NodeManager {
public:
	NodeManager() :
			nodeServer_(0) {
	}
	bool hasNode(uint id);
	bool find(uint id, InfoNodePtr& infoNodePtr);
	std::vector<uint> getNodeIDs(ObjectType* objectType);
	std::vector<uint> getNodeIDs(std::string& type);
	friend class ObjectType;
private:
	void setNodeServer(NodeServer* nodeServer) {
		nodeServer_ = nodeServer;
	}
	NodeServer* nodeServer_;
};
}

#endif /* NODEMANAGER_H_ */
