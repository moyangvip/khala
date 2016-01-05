/*
 * ChatType.cpp
 *
 *  Created on: 2015年12月21日
 *      Author: moss
 */

#include "ChatType.h"
#include "UsrInfo.h"
#include "MyParseKey.h"
#include <khala/NodeManager.h>
//#include"DAO/UsrInfoDao.h"
ChatType::ChatType() {
	// TODO Auto-generated constructor stub

}

ChatType::~ChatType() {
	// TODO Auto-generated destructor stub
}
bool ChatType::onLoginCheckMsg(khala::InfoNodePtr& infoNodePtr,
		Json::Value& msg, khala::Timestamp time) {
	std::string name = msg[KEY_LOGIN_NAME].asString();
	std::string passwd = msg[KEY_PASSWD].asString();
	/*
	UsrInfoResult res = UsrInfoDao::getInstance().queryUsrInfo(name, passwd);
	if (!res.isSuccess()) {
		Json::Value res;
		res[MSG_TYPE] = LOGIN_TYPE;
		res["result"] = "false";
		res["data"] = "login failure,err account!";
		Json::FastWriter jwriter;
		std::string sendStr = jwriter.write(res);
		infoNodePtr->send(sendStr);
		return false;
	}
	UsrInfoDo usrInfoDo = res.getUsrInfoDo();
	infoNodePtr->setId(usrInfoDo.id);*/
	return true;
}
bool ChatType::onLoginSuccessMsg(khala::InfoNodePtr& infoNodePtr,
		Json::Value& msg, khala::Timestamp time) {
	std::string name = msg[KEY_LOGIN_NAME].asString();
	infoNodePtr->setExtraContext(new UsrInfo(name));
	Json::Value res;
	res[MSG_TYPE] = LOGIN_TYPE;
	res["result"] = "true";
	res["data"] = "login success!";
	res["name"] = name;
	Json::FastWriter jwriter;
	std::string sendStr = jwriter.write(res);
	infoNodePtr->send(sendStr);
	return true;
}
bool ChatType::onLoginFailureMsg(khala::InfoNodePtr& infoNodePtr,
		Json::Value& msg, khala::Timestamp time) {
	Json::Value res;
	res[MSG_TYPE] = LOGIN_TYPE;
	res["result"] = "false";
	res["data"] = "login failure,logined id!";
	Json::FastWriter jwriter;
	std::string sendStr = jwriter.write(res);
	infoNodePtr->send(sendStr);
	return true;
}
bool ChatType::onLogoutMsg(khala::InfoNodePtr& infoNodePtr, Json::Value& msg,
		khala::Timestamp time) {
	Json::Value res;
	res[MSG_TYPE] = LOGOUT_TYPE;
	res["result"] = "true";
	res["data"] = "logout success!";
	Json::FastWriter jwriter;
	std::string sendStr = jwriter.write(res);
	infoNodePtr->send(sendStr);
	return true;
}
void ChatType::releaseConnNode(khala::InfoNodePtr& infoNodePtr,
		khala::Timestamp time) {
	UsrInfo* usrInfo = boost::any_cast<UsrInfo*>(
			infoNodePtr->getExtraContext());
	if (usrInfo != 0) {
		delete usrInfo;
		usrInfo = 0;
	}
}

const std::string& ChatType::getObjectTypeName() {
	static std::string typeStr(MY_CHAT_TYPE);
	return typeStr;
}
void ChatType::setRegisterMsg(khala::RegisterHandler& handler) {
	NodeType::setRegisterMsg(handler);
	handler.setRegisterMsg(CHAT_FRIENDS,
			boost::bind(&ChatType::onCurrFriends, this, _1, _2, _3));
	handler.setRegisterMsg(CHAT_SEND,
			boost::bind(&ChatType::onSendMsg, this, _1, _2, _3));
}
bool ChatType::onCurrFriends(khala::InfoNodePtr& infoNodePtr, Json::Value& msg,
		khala::Timestamp time) {
	Json::Value res;
	res[MSG_TYPE] = CHAT_FRIENDS;
	res["result"] = "true";
	khala::NodeManager* nodeManager = getNodeManager();
	std::vector<uint> currIds = nodeManager->getNodeIDs(
			infoNodePtr->getNodeType());
	for (std::vector<uint>::iterator it = currIds.begin(); it != currIds.end();
			++it) {
		khala::InfoNodePtr usrInfoNode;
		if (nodeManager->find(*it, usrInfoNode)) {
			Json::Value usrJs;
			usrJs[KEY_ID] = *it;
			UsrInfo* usrInfo = boost::any_cast<UsrInfo*>(
					usrInfoNode->getExtraContext());
			usrJs["name"] = usrInfo->getName();
			res["data"].append(usrJs);
		}
	}
	Json::FastWriter jwriter;
	std::string sendStr = jwriter.write(res);
	infoNodePtr->send(sendStr);
	return true;
}
bool ChatType::onSendMsg(khala::InfoNodePtr& infoNodePtr, Json::Value& msg,
		khala::Timestamp time) {
	Json::FastWriter jwriter1;
	std::string sendStr1 = jwriter1.write(msg);
	Json::Value res;
	res[MSG_TYPE] = CHAT_SEND;
	uint friendId = msg[KEY_FRIEND_ID].asUInt();
	std::string sendMsg = msg[CHAT_MSG].asString();
	res[FRIEND_NAME] = msg[FRIEND_NAME].asString();
	res[CHAT_MSG] = sendMsg;
	khala::InfoNodePtr friendNodePtr;
	if (this->getNodeManager()->find(friendId, friendNodePtr)) {
		Json::Value sendJs;
		sendJs[MSG_TYPE] = CHAT_REV;
		sendJs[CHAT_MSG] = sendMsg;
		UsrInfo* usrInfo = boost::any_cast<UsrInfo*>(
				infoNodePtr->getExtraContext());
		if (usrInfo == 0) {
			return false;
		}
		sendJs[SEND_NAME] = usrInfo->getName();
		sendJs[KEY_FRIEND_ID] = infoNodePtr->getId();
		Json::FastWriter jwriter;
		std::string sendStr = jwriter.write(sendJs);
		friendNodePtr->send(sendStr);

		res["result"] = "true";
	} else {
		res["result"] = "false";
	}
	Json::FastWriter jwriter;
	std::string sendStr = jwriter.write(res);
	infoNodePtr->send(sendStr);
	return true;
}
