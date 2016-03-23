/*
 * MyUsrType.cpp
 *
 *  Created on: 2015年12月18日
 *      Author: moss
 */

#include "MyUsrType.h"
#include "UsrInfo.h"
#include "MyParseKey.h"
#include <sstream>

MyUsrType::MyUsrType() {


}

MyUsrType::~MyUsrType() {

}
bool MyUsrType::onLoginCheckMsg(khala::InfoNodePtr& infoNodePtr,
		Json::Value& msg, khala::Timestamp time) {
	std::string name = msg[KEY_LOGIN_NAME].asString();
	std::string passwd = msg[KEY_PASSWD].asString();
	/*
	 * you can use  your database to check
	 *
	 UsrInfoResult res = UsrInfoDao::getInstance().queryUsrInfo(name, passwd);
	 if (!res.isSuccess()) {
	 infoNodePtr->send("login failure,err account!");
	 return false;
	 }
	 UsrInfoDo usrInfoDo = res.getUsrInfoDo();
	 infoNodePtr->setId(usrInfoDo.id);
	 */
	return true;
}
bool MyUsrType::onLoginSuccessMsg(khala::InfoNodePtr& infoNodePtr,
		Json::Value& msg, khala::Timestamp time) {
	std::string name = msg[KEY_LOGIN_NAME].asString();
	infoNodePtr->setExtraContext(new UsrInfo(name));
	std::stringstream ss;
	ss << "login success! your id is:" << infoNodePtr->getId();
	infoNodePtr->send(ss.str());
	return true;
}
bool MyUsrType::onLoginFailureMsg(khala::InfoNodePtr& infoNodePtr,
		Json::Value& msg, khala::Timestamp time) {
	infoNodePtr->send("login failure,logined id!");
	return true;
}
bool MyUsrType::onLogoutMsg(khala::InfoNodePtr& infoNodePtr, Json::Value& msg,
		khala::Timestamp time) {
	std::stringstream ss;
	ss << "logout success! your id is:" << infoNodePtr->getId();
	infoNodePtr->send(ss.str());
	return true;
}
void MyUsrType::releaseConnNode(khala::InfoNodePtr& infoNodePtr,
		khala::Timestamp time) {
	UsrInfo* usrInfo = boost::any_cast<UsrInfo*>(
			infoNodePtr->getExtraContext());
	if (usrInfo != 0) {
		delete usrInfo;
		usrInfo = 0;
	}
}
const std::string& MyUsrType::getObjectTypeName() {
	static std::string typeStr(MY_USR_TYPE);
	return typeStr;
}
void MyUsrType::setRegisterMsg(khala::RegisterHandler& handler) {
	NodeType::setRegisterMsg(handler);
	handler.setRegisterMsg(ADD_FRIEND,
			boost::bind(&MyUsrType::onAddFriend, this, _1, _2, _3));
	handler.setRegisterMsg(GET_FRIENDS,
			boost::bind(&MyUsrType::onGetFriends, this, _1, _2, _3));
}
bool MyUsrType::onAddFriend(khala::InfoNodePtr& infoNodePtr, Json::Value& msg,
		khala::Timestamp time) {
	uint friendId = msg[KEY_FRIEND_ID].asUInt();
	UsrInfo* usrInfo = boost::any_cast<UsrInfo*>(
			infoNodePtr->getExtraContext());
	if (usrInfo == 0)
		return false;
	usrInfo->addFriends(friendId);
	std::stringstream ss;
	ss << "add friend id:" << friendId << " success !";
	infoNodePtr->send(ss.str());
	return true;
}
bool MyUsrType::onGetFriends(khala::InfoNodePtr& infoNodePtr, Json::Value& msg,
		khala::Timestamp time) {
	UsrInfo* usrInfo = boost::any_cast<UsrInfo*>(
			infoNodePtr->getExtraContext());
	if (usrInfo == 0)
		return false;
	std::vector<uint> friends = usrInfo->getAllFriends();
	std::stringstream ss;
	ss << "my friends id:";
	std::vector<uint>::iterator it = friends.begin();
	for (; it != friends.end(); it++) {
		ss << *it << " ";
	}
	infoNodePtr->send(ss.str());
	return true;
}
