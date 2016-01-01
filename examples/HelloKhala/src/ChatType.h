/*
 * ChatType.h
 *
 *  Created on: 2015年12月21日
 *      Author: moss
 */

#ifndef CHATTYPE_H_
#define CHATTYPE_H_
#include <khala/NodeType.h>
class ChatType:  public khala::NodeType{
public:
	ChatType();
	virtual ~ChatType();
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
		virtual bool onCurrFriends(khala::InfoNodePtr& infoNodePtr, Json::Value& msg,
						khala::Timestamp time) ;
		virtual bool onSendMsg(khala::InfoNodePtr& infoNodePtr, Json::Value& msg,
								khala::Timestamp time) ;
};

#endif /* CHATTYPE_H_ */
