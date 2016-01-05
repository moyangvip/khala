/*
 * MyNodeType.cpp
 *
 *  Created on: 2015年12月16日
 *      Author: moss
 */

#include "MyNodeType.h"
#include "MyParseKey.h"
MyNodeType::MyNodeType() {
	// TODO Auto-generated constructor stub

}

MyNodeType::~MyNodeType() {
	// TODO Auto-generated destructor stub
}
bool MyNodeType::onMyEventMsg(khala::InfoNodePtr& infoNodePtr,
		Json::Value& msg, khala::Timestamp time) {
	infoNodePtr->send("this is my new Node !");
	return true;
}
 void MyNodeType::setRegisterMsg(khala::RegisterHandler& handler) {
	khala::NodeType::setRegisterMsg(handler);
	handler.setRegisterMsg(MY_EVENT,
			boost::bind(&MyNodeType::onMyEventMsg, this, _1, _2, _3));
}
const std::string& MyNodeType::getObjectTypeName() {
	static std::string typeStr(MY_NODE_TYPE);
	return typeStr;
}
