/*
 * MyNodeType.h
 *
 *  Created on: 2015年12月16日
 *      Author: moss
 */

#ifndef MYNODETYPE_H_
#define MYNODETYPE_H_

#include <khala/NodeType.h>

class MyNodeType: public khala::NodeType {
public:
	MyNodeType();
	virtual ~MyNodeType();
	virtual bool onMyEventMsg(khala::InfoNodePtr& infoNodePtr, Json::Value& msg,
			khala::Timestamp time);
	virtual void setRegisterMsg(khala::RegisterHandler& handler);
	virtual const std::string& getObjectTypeName() ;
};

#endif /* MYNODETYPE_H_ */
