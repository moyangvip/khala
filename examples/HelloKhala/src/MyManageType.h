/*
 * MyManageType.h
 *
 *  Created on: 2015年12月27日
 *      Author: moss
 */

#ifndef MYMANAGETYPE_H_
#define MYMANAGETYPE_H_
#include <khala/NodeType.h>
class MyManageType: public khala::NodeType{
public:
	MyManageType();
	virtual ~MyManageType();
	virtual const std::string& getObjectTypeName() ;
	virtual void setRegisterMsg(khala::RegisterHandler& handler);
	//this is my evnt functions
	virtual bool onGetIDByType(khala::InfoNodePtr& infoNodePtr, Json::Value& msg,
			khala::Timestamp time) ;
	virtual bool onGetIDByObject(khala::InfoNodePtr& infoNodePtr, Json::Value& msg,
			khala::Timestamp time) ;
	virtual bool onGetAllIDByObject(khala::InfoNodePtr& infoNodePtr, Json::Value& msg,
				khala::Timestamp time) ;
	virtual bool onGetAllIDByObjectPtr(khala::InfoNodePtr& infoNodePtr, Json::Value& msg,
					khala::Timestamp time) ;
	virtual bool onAskLogin(khala::InfoNodePtr& infoNodePtr, Json::Value& msg,
						khala::Timestamp time) ;
	virtual bool onSendtoNode(khala::InfoNodePtr& infoNodePtr, Json::Value& msg,
			khala::Timestamp time) ;
};

#endif /* MYMANAGETYPE_H_ */
