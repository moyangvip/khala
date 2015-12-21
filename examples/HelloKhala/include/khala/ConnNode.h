/*
 * TempNode.h
 *
 *  Created on: Dec 3, 2015
 *      Author: moss
 */

#ifndef COLLECTION_TEMPNODE_H_
#define COLLECTION_TEMPNODE_H_
#include <khala/ConnID.h>
#include <boost/shared_ptr.hpp>
namespace khala {
enum LoginStatus {
	NO_LOGIN_STATUS, LOGIN_STATUS
};

class ConnNode {
public:
	ConnNode(const TcpConnectionPtr& conn, Timestamp time);
	~ConnNode();
	TcpConnectionPtr& getTcpConnectionPtr();
	Timestamp getLastReceiveTime();
	void setLastReceiveTime(Timestamp time);
	uint getTmpId() const;
	void setId(uint id);
	uint getId() const;
	LoginStatus getStatus();

	void setStatus(LoginStatus loginStatus);
	void send(const std::string& msg);
private:
	//careful conn life time
	TcpConnectionPtr conn_;
	//connection time
	Timestamp lastReceiveTime_;
	ConnID connID_;
	LoginStatus loginStatus_;
};
typedef boost::shared_ptr<ConnNode> ConnNodePtr;
}

#endif /* COLLECTION_TEMPNODE_H_ */
