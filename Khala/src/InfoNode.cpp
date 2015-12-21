/*
 * ConnNode.cpp
 *
 *  Created on: Dec 3, 2015
 *      Author: moss
 */
#include <khala/InfoNode.h>
#include <khala/ParseKey.h>
#include <khala/ObjectType.h>
using namespace khala;
InfoNode::InfoNode(ConnNodePtr& connNodePtr, Timestamp time, uint id ) :
		connNodePtr_(connNodePtr), objectType_(TEMP_NODE_TYPE) {
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
InfoNode::~InfoNode() {
	connNodePtr_->setId(DEFAULT_ID);
}

TcpConnectionPtr& InfoNode::getConn() {
	return connNodePtr_->getTcpConnectionPtr();
}
uint InfoNode::getId() const {
	return connNodePtr_->getId();
}
/*
 * if you use TmpId as InfoNode key,never invoking setID
 * */
void InfoNode::setId(uint id) {
	connNodePtr_->setId(id);
}

const std::string& InfoNode::getNodeType() const {
	return objectType_;
}

void InfoNode::send(const std::string& msg) {
	connNodePtr_->send(msg);
}
LoginStatus InfoNode::getStatus() {
	return connNodePtr_->getStatus();
}

void InfoNode::setStatus(LoginStatus loginStatus) {
	connNodePtr_->setStatus(loginStatus);
}
/*
 * you can new object to extraContext_ when you login
 * pay attention to release it when node release
 * you can use shared_ptr to control your object's life cycle
 * like :
 * shared_ptr<UsrObject> ptr(new UsrObject);
 * infoNode.setExtraContext(ptr);
 */
void InfoNode::setExtraContext(const boost::any& context) {
	extraContext_ = context;
}

const boost::any& InfoNode::getExtraContext() const {
	return extraContext_;
}
