/*
 * ConnNode.h
 *
 *  Created on: Dec 3, 2015
 *      Author: moss
 */

#ifndef COLLECTION_CONNNODE_H_
#define COLLECTION_CONNNODE_H_
#include <khala/ConnNode.h>
namespace khala {

class ObjectType;
class InfoNode {
public:
	InfoNode(ConnNodePtr& connNodePtr, Timestamp time, uint id =
	DEFAULT_ID);
	/*
	 * conn's life cycle is bind with connNode,and connNode's life cycle is bind with
	 * InfoNode,if InfoNode is destructed,and connNode has no other ref,connNode will
	 * be destructed too,and forceClose conn
	 * */
	~InfoNode();

	TcpConnectionPtr& getConn();
	uint getId() const;
	/*
	 * if you use TmpId as InfoNode key,never invoking setID
	 * */
	void setId(uint id);

	const std::string& getNodeType() const;

	void setNodeType(ObjectType* objectType);
	void send(const std::string& msg);
	LoginStatus getStatus();

	void setStatus(LoginStatus loginStatus);
	/*
	 * you can new object to extraContext_ when you login
	 * pay attention to release it when node release
	 * you can use shared_ptr to control your object's life cycle
	 * like :
	 * shared_ptr<UsrObject> ptr(new UsrObject);
	 * infoNode.setExtraContext(ptr);
	 */
	void setExtraContext(const boost::any& context);

	const boost::any& getExtraContext() const;
private:
	//if connNode is login,the conn's life cycle will be bind with the InfoNode
	ConnNodePtr connNodePtr_;
	//default NodeType Instance as NodeType
	std::string objectType_;
	/*
	 * you can new object to extraContext_ when you login,pay attention to release it when node release
	 * you should use shared_ptr to control your object's life cycle
	 */
	boost::any extraContext_;

};
typedef boost::shared_ptr<InfoNode> InfoNodePtr;
}

#endif /* COLLECTION_CONNNODE_H_ */
