/*
 * MyManageType.cpp
 *
 *  Created on: 2015年12月27日
 *      Author: moss
 */

#include "MyManageType.h"
#include "MyParseKey.h"
MyManageType::MyManageType() {


}

MyManageType::~MyManageType() {

}
const std::string& MyManageType::getObjectTypeName() {
	static std::string typeStr(MY_MANAGE_TYPE);
	return typeStr;
}
bool MyManageType::onGetIDByType(khala::InfoNodePtr& infoNodePtr,
		Json::Value& msg, khala::Timestamp time) {
	std::vector<uint> v = this->getNodeManager()->getNodeIDs(
			infoNodePtr->getNodeType());
	std::stringstream ss;
	ss << "Get ID By Type:Ask type:" << infoNodePtr->getNodeType()
			<< " . Get ID:";
	for (std::vector<uint>::iterator it = v.begin(); it != v.end(); ++it) {
		ss << *it << " ";
	}
	infoNodePtr->send(ss.str());
	return true;
}
bool MyManageType::onGetIDByObject(khala::InfoNodePtr& infoNodePtr,
		Json::Value& msg, khala::Timestamp time) {
	MyManageType* tmp = new MyManageType();
	std::vector<uint> v = this->getNodeManager()->getNodeIDs(tmp);
	std::stringstream ss;
	ss << "Get ID By Object:Ask Object type:" << tmp->getObjectTypeName()
			<< " . Get ID:";
	for (std::vector<uint>::iterator it = v.begin(); it != v.end(); ++it) {
		ss << *it << " ";
	}
	infoNodePtr->send(ss.str());
	delete tmp;
	return true;
}
bool MyManageType::onGetAllIDByObject(khala::InfoNodePtr& infoNodePtr,
		Json::Value& msg, khala::Timestamp time) {
	NodeType* tmp = new NodeType();
	//parameter is NodeType Object
	std::vector<uint> v = this->getNodeManager()->getAllNodeIDs(*tmp);
	std::stringstream ss;
	ss << "Get All ID By Object:Ask Object type:" << tmp->getObjectTypeName()
			<< " . Get ID:";
	for (std::vector<uint>::iterator it = v.begin(); it != v.end(); ++it) {
		ss << *it << " ";
	}
	infoNodePtr->send(ss.str());
	delete tmp;
	return true;
}
bool MyManageType::onGetAllIDByObjectPtr(khala::InfoNodePtr& infoNodePtr,
		Json::Value& msg, khala::Timestamp time) {
	NodeType* tmp = new NodeType();
	//parameter is NodeType Object pointor
	std::vector<uint> v = this->getNodeManager()->getAllNodeIDs(tmp);
	std::stringstream ss;
	ss << "Get All ID By Object ptr:Ask Object type:"
			<< tmp->getObjectTypeName() << " . Get ID:";
	for (std::vector<uint>::iterator it = v.begin(); it != v.end(); ++it) {
		ss << *it << " ";
	}
	infoNodePtr->send(ss.str());
	delete tmp;
	return true;
}
bool MyManageType::onAskLogin(khala::InfoNodePtr& infoNodePtr, Json::Value& msg,
		khala::Timestamp time){
	uint friendId = msg[KEY_FRIEND_ID].asUInt();
	std::stringstream ss;
	ss<<"Node ID:"<<friendId;
	if(this->getNodeManager()->hasNode(friendId)){
		ss<<" is login!";
	}else{
		ss<<" is not login!";
	}
	infoNodePtr->send(ss.str());
	return true;
}
bool MyManageType::onSendtoNode(khala::InfoNodePtr& infoNodePtr,
		Json::Value& msg, khala::Timestamp time){
	uint myId = infoNodePtr->getId();
	khala::InfoNodePtr myInfoNodePtr;
	if(this->getNodeManager()->find(myId, myInfoNodePtr)){
		myInfoNodePtr->send("test manager find success");
	}
	return true;
}
void MyManageType::setRegisterMsg(khala::RegisterHandler& handler){
	NodeType::setRegisterMsg(handler);
	handler.setRegisterMsg(ID_BY_TYPE,
					boost::bind(&MyManageType::onGetIDByType, this, _1, _2, _3));
	handler.setRegisterMsg(ID_BY_OBJECT,
			boost::bind(&MyManageType::onGetIDByObject, this, _1, _2, _3));
	handler.setRegisterMsg(ALL_ID_BY_OBJECT,
				boost::bind(&MyManageType::onGetAllIDByObject, this, _1, _2, _3));
	handler.setRegisterMsg(ALL_ID_BY_OBJECT_PTR,
				boost::bind(&MyManageType::onGetAllIDByObjectPtr, this, _1, _2, _3));
	handler.setRegisterMsg(ASK_LOGIN,
				boost::bind(&MyManageType::onAskLogin, this, _1, _2, _3));
	handler.setRegisterMsg(SEND_MSG,
					boost::bind(&MyManageType::onSendtoNode, this, _1, _2, _3));
}
