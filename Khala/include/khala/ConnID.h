/*
 * ConnID.h
 *
 *  Created on: Dec 6, 2015
 *      Author: moss
 */

#ifndef CONNID_H_
#define CONNID_H_
#include <khala/Keywords.h>
#include <muduo/net/TcpServer.h>
namespace khala {
class ConnID {
public:
	ConnID() :
			tmpId_(DEFAULT_ID), id_(DEFAULT_ID) {
	}
	uint getId() const {
		return id_;
	}

	void setId(uint id) {
		this->id_ = id;
	}

	uint getTmpId() const {
		return tmpId_;
	}

	void setTmpId(uint tmpId) {
		this->tmpId_ = tmpId;
	}

private:
	uint tmpId_;
	uint id_;
};
inline void SET_TMP_ID(const muduo::net::TcpConnectionPtr& conn, uint id) {
	ConnID* connID = boost::any_cast<ConnID*>(conn->getContext());
	if (connID != 0) {
		connID->setTmpId(id);
	}
}
inline void SET_ID(const muduo::net::TcpConnectionPtr& conn, uint id) {
	ConnID* connID = boost::any_cast<ConnID*>(conn->getContext());
	if (connID != 0) {
		connID->setId(id);
	}

}
inline uint TMP_ID_CONN(const muduo::net::TcpConnectionPtr& conn) {
	ConnID* connID = boost::any_cast<ConnID*>(conn->getContext());
	if (connID != 0) {
		return connID->getTmpId();
	}
	return DEFAULT_ID;
}
inline uint ID_CONN(const muduo::net::TcpConnectionPtr& conn) {
	ConnID* connID = boost::any_cast<ConnID*>(conn->getContext());
	if (connID != 0) {
		return connID->getId();
	}
	return DEFAULT_ID;
}
}

#endif /* CONNID_H_ */
