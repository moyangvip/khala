/*
 * TempNodeType.h
 *
 *  Created on: Dec 8, 2015
 *      Author: moss
 */

#ifndef TEMPNODETYPE_H_
#define TEMPNODETYPE_H_
#include <khala/ObjectType.h>
#include <khala/ParseKey.h>
namespace khala {
typedef muduo::Timestamp Timestamp;
class TempNodeType: public ObjectType {
public:

	TempNodeType() {
	}
	virtual ~TempNodeType() {
	}
	/*
	 * if you want to set TempNodeType as Parent class,you shouldn't use getInstance
	 * because TempNodeType may be override,and getInstance() is static
	 * you can use like NodeType
	 * */
	static TempNodeType* getInstance() {
		static TempNodeType tempNodeType;
		return &tempNodeType;
	}

	virtual const std::string& getObjectTypeName() {
		static std::string typeStr(TEMP_NODE_TYPE);
		return typeStr;
	}
	/*
	 * if receive any no register type msg with this nodeType,will invoke this function
	 * */
	virtual void onErrTypeMessage(InfoNodePtr& infoNodePtr, Json::Value& msg,
			Timestamp time) {
		infoNodePtr->send("err type req!");
	}
	/*
	 * if  any msg handler return false,will invoke this function
	 * */
	virtual void onErrRunMessage(InfoNodePtr& infoNodePtr, Json::Value& msg,
			Timestamp time) {
		infoNodePtr->send("err,running err!");
	}
	virtual void setRegisterMsg(RegisterHandler& handler);
	/*
	 * you can get node type,you can override it
	 * */
	virtual bool onDevTypeMsg(InfoNodePtr& infoNodePtr, Json::Value& msg,
			Timestamp time);
	/*
	 * you can get login status,you can override it
	 * */
	virtual bool onIsLoginMsg(InfoNodePtr& infoNodePtr, Json::Value& msg,
			Timestamp time);
private:
	bool onLoginMsg_(InfoNodePtr& infoNodePtr, Json::Value& msg,
			Timestamp time);
	void onLoginTypeCheck(InfoNodePtr& infoNodePtr, Json::Value& msg);
};
}

#endif /* TEMPNODETYPE_H_ */
