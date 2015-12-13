/*
 * ConnNode.cpp
 *
 *  Created on: Dec 3, 2015
 *      Author: moss
 */
#include <khala/InfoNode.h>
#include <khala/NodeType.h>
using namespace khala;
InfoNode::InfoNode(ConnNodePtr& connNodePtr, Timestamp time, uint id) :
		connNodePtr_(connNodePtr), objectType_(TEMP_NODE_TYPE){
	//set TmpId as default id key
	if (id == DEFAULT_ID) {
		connNodePtr_->setId(connNodePtr->getTmpId());
	} else {
		connNodePtr_->setId(id);
	}
}
void InfoNode::setNodeType(ObjectType* objectType) {
	objectType_ = objectType->getObjectTypeName();
}
