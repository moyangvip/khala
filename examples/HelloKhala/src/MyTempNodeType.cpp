/*
 * MyTempNodeType.cpp
 *
 *  Created on: 2015年12月16日
 *      Author: moss
 */

#include "MyTempNodeType.h"
#include "MyParseKey.h"
MyTempNodeType::MyTempNodeType() {
	// TODO Auto-generated constructor stub

}

MyTempNodeType::~MyTempNodeType() {
	// TODO Auto-generated destructor stub
}
bool MyTempNodeType::onDevTypeMsg(khala::InfoNodePtr& infoNodePtr,
		Json::Value& msg, khala::Timestamp time) {
	std::string devType = infoNodePtr->getNodeType();
	infoNodePtr->send("this is my devType:" + devType);
	return true;
}
bool MyTempNodeType::onTimeMsg(khala::InfoNodePtr& infoNodePtr,
		Json::Value& msg, khala::Timestamp time) {
	std::string timeStr = khala::Timestamp::now().toFormattedString();
	infoNodePtr->send("curr time:" + timeStr);
	return true;
}
void MyTempNodeType::setRegisterMsg(khala::RegisterHandler& handler) {
	khala::TempNodeType::setRegisterMsg(handler);
	handler.setRegisterMsg(TIME_TYPE,
			boost::bind(&MyTempNodeType::onTimeMsg, this, _1, _2, _3));
}
