/*
 * NodeType.h
 *
 *  Created on: Dec 8, 2015
 *      Author: moss
 */

#ifndef NODETYPE_H_
#define NODETYPE_H_
#include <khala/ObjectType.h>
namespace khala {
class NodeType: public ObjectType {

public:
	NodeType();
	virtual ~NodeType();
	static NodeType* getInstance();
	/*
	 * you can do sth as logining checking login with databases,
	 * you can invoke InfoNodePtr->setId() and set a UNIQUE id as the conn key
	 * or default use connNode's tmpID as id key(i promise login success:->)
	 * if check false,just return false,and onLoginSuccessMsg will not be invoked
	 * if you return true,not sure login success,if you set node id is not unique,logining will be failure
	 * */
	virtual bool onLoginCheckMsg(InfoNodePtr&, Json::Value& msg,
			Timestamp time);
	/*
	 * if you login success...
	 * */
	virtual bool onLoginSuccessMsg(InfoNodePtr& infoNodePtr, Json::Value& msg,
			Timestamp time);
	/*
	 * if you login failure,such as logined id
	 * */
	virtual bool onLoginFailureMsg(InfoNodePtr& infoNodePtr, Json::Value& msg,
			Timestamp time);
	/*
	 * you can do sth when logout
	 * after this function,will invoke destroyConnNode() to release node resources
	 * */
	virtual bool onLogoutMsg(InfoNodePtr& infoNodePtr, Json::Value& msg,
			Timestamp time);
	/*
	 * destroy node resources.
	 * if you do not set any node resources,do nothing
	 * can not use InfoNodePtr->getConn() to send sth;
	 * */
	virtual void releaseConnNode(InfoNodePtr& infoNodePtr, Timestamp time);
	/*
	 * you can register msg at here
	 * for example:
	 * you can create a function as onLoginMsg:
	 * bool onLoginMsg(ConnNodePtr& connNode, Json::Value& msg,muduo::Timestamp time)
	 * and you can register this function like:
	 * handler.setRegisterMsg(LOGIN_TYPE, boost::bind(&NodeType::onLoginMsg, this, _1, _2, _3));
	 * when you receive a msg type as LOGIN_TYPE,the function setRegisterMsg will be invoking
	 * Notice:LOGIN_TYPE and LOGOUT_TYPE are Reserved words,you can't set as TYPE str
	 * */
	virtual void setRegisterMsg(RegisterHandler& handler);
	/*
	 * you must set a unique string type as the NodeType's key!
	 */
	virtual const std::string& getObjectTypeName();
	/*
	 * how to process heart msg,you can override it
	 * */
	virtual bool onHeartMsg(InfoNodePtr& infoNodePtr, Json::Value& msg,
				Timestamp time);
	/*
	 * you can get node id,you can override it
	 * */
	virtual bool onNodeIdMsg(InfoNodePtr& infoNodePtr, Json::Value& msg,
			Timestamp time);
private:

	bool onLogoutMsg_(InfoNodePtr& infoNodePtr, Json::Value& msg,
			Timestamp time);

};
}
#endif /* NODETYPE_H_ */
