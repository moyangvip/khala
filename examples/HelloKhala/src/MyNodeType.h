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
			khala::Timestamp time){
		infoNodePtr->send("this is my new Node !");
		return true;
	}
	virtual void setRegisterMsg(khala::RegisterHandler& handler){
		khala::NodeType::setRegisterMsg(handler);
		handler.setRegisterMsg(MY_EVENT,
					boost::bind(&MyNodeType::onMyEventMsg, this, _1, _2, _3));
	}
	virtual const std::string& getObjectTypeName() {
			static std::string typeStr(MY_NODE_TYPE);
			return typeStr;
		}
};

#endif /* MYNODETYPE_H_ */
