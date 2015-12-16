/*
 * MyTempNodeType.h
 *
 *  Created on: 2015年12月16日
 *      Author: moss
 */

#ifndef MYTEMPNODETYPE_H_
#define MYTEMPNODETYPE_H_

#include <khala/TempNodeType.h>
class MyTempNodeType: public khala::TempNodeType {
public:
	MyTempNodeType();
	virtual ~MyTempNodeType();
	virtual bool onDevTypeMsg(khala::InfoNodePtr& infoNodePtr, Json::Value& msg,
			khala::Timestamp time);
	virtual bool onTimeMsg(khala::InfoNodePtr& infoNodePtr, Json::Value& msg,
			khala::Timestamp time);
	virtual void setRegisterMsg(khala::RegisterHandler& handler);
};

#endif /* MYTEMPNODETYPE_H_ */
