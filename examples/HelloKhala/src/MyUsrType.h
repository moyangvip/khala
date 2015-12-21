/*
 * MyUsrType.h
 *
 *  Created on: 2015年12月18日
 *      Author: moss
 */

#ifndef MYUSRTYPE_H_
#define MYUSRTYPE_H_
#include <khala/NodeType.h>
class MyUsrType: public khala::NodeType {
public:
	MyUsrType();
	virtual ~MyUsrType();
	virtual bool onLoginCheckMsg(khala::InfoNodePtr& infoNodePtr,
			Json::Value& msg, khala::Timestamp time) ;
	virtual bool onLoginSuccessMsg(khala::InfoNodePtr& infoNodePtr,
			Json::Value& msg, khala::Timestamp time) ;
	virtual bool onLoginFailureMsg(khala::InfoNodePtr& infoNodePtr,
			Json::Value& msg, khala::Timestamp time) ;
	virtual bool onLogoutMsg(khala::InfoNodePtr& infoNodePtr, Json::Value& msg,
			khala::Timestamp time) ;
	virtual void releaseConnNode(khala::InfoNodePtr& infoNodePtr,
			khala::Timestamp time) ;
	virtual const std::string& getObjectTypeName();
	virtual void setRegisterMsg(khala::RegisterHandler& handler);
	//this is my evnt functions
	virtual bool onAddFriend(khala::InfoNodePtr& infoNodePtr, Json::Value& msg,
			khala::Timestamp time) ;
	virtual bool onGetFriends(khala::InfoNodePtr& infoNodePtr, Json::Value& msg,
			khala::Timestamp time) ;
};

#endif /* MYUSRTYPE_H_ */
