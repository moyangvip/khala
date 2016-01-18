/*
 * ConnNode.cpp
 *
 *  Created on: Dec 3, 2015
 *      Author: moss
 */
#include <khala/InfoNode.h>
#include <khala/ParseKey.h>
#include <khala/HeadCode.h>
#include <khala/ObjectType.h>
#include <khala/RandomOnlyID.h>
#include <muduo/base/Logging.h>
using namespace khala;
InfoNode::InfoNode(const TcpConnectionPtr& conn) :
		conn_(conn), loginStatus_(NO_LOGIN_STATUS), connIDPtr_(new ConnID()) {
	conn_->setContext((ConnIDPtr*) &connIDPtr_);
	//set a only key as temp id
	setTempId(RandomOnlyID::getInstance().getOnlyID());
	//default TempNodeType Instance as NodeType
	connIDPtr_->setNodeType(TEMP_NODE_TYPE);
}
InfoNode::~InfoNode() {
	if (loginStatus_ == NO_LOGIN_STATUS) {
		LOG_INFO << conn_->peerAddress().toIpPort() << " temp ID:"
				<< connIDPtr_->getTmpId() << " ConnNode will be destroy!";
	} else {
		LOG_INFO << conn_->peerAddress().toIpPort() << " ID:"
				<< connIDPtr_->getId() << " temp ID:" << connIDPtr_->getTmpId()
				<< " ConnNode will be destroy!";
	}
	//release temp ID
	uint tempId = getTempId();
	setTempId(DEFAULT_ID);
	RandomOnlyID::getInstance().releaseID(tempId);
	//close TcpConnectionPtr
	conn_->forceClose();
}
void InfoNode::setNodeType(ObjectType* objectType) {
	connIDPtr_->setNodeType(objectType->getObjectTypeName());
}
TcpConnectionPtr& InfoNode::getConn() {
	return conn_;
}
uint InfoNode::getId() const {
	return connIDPtr_->getId();
}
/*
 * if you use TmpId as InfoNode key,never invoking setID
 * */
void InfoNode::setId(uint id) {
	connIDPtr_->setId(id);
}
uint InfoNode::getTempId() const {
	return connIDPtr_->getTmpId();
}
void InfoNode::setTempId(uint id) {
	connIDPtr_->setTmpId(id);
}
const std::string& InfoNode::getNodeType() const {
	return connIDPtr_->getNodeType();
}

void InfoNode::send(const std::string& msg, bool addHead) {
	if (addHead) {
		HeadCode* headCode = HeadCode::getInstance();
		muduo::net::Buffer buff = headCode->addHeadCode(msg);
		conn_->send(&buff);
	} else {
		conn_->send(msg);
	}
}
LoginStatus InfoNode::getStatus() {
	return loginStatus_;
}

void InfoNode::setStatus(LoginStatus loginStatus) {
	loginStatus_ = loginStatus;
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
