/*
 * ConnID.h
 *
 *  Created on: Dec 6, 2015
 *      Author: moss
 */

#ifndef CONNID_H_
#define CONNID_H_
#include <khala/Keywords.h>
#include <boost/shared_ptr.hpp>
namespace khala {
class ConnID {
public:
	ConnID();
	uint getId() const;

	void setId(uint id);

	uint getTmpId() const;

	void setTmpId(uint tmpId);
	const std::string& getNodeType() const;
	void setNodeType(const std::string& objectType);
private:
	uint tmpId_;
	uint id_;
	std::string objectType_;
};
typedef boost::shared_ptr<ConnID> ConnIDPtr;
inline void SET_TMP_ID(const TcpConnectionPtr& conn, uint id) {
	ConnIDPtr* p = boost::any_cast<ConnIDPtr*>(conn->getContext());
	if (p != 0) {
		(*p)->setTmpId(id);
	}
}
inline void SET_ID(const TcpConnectionPtr& conn, uint id) {
	ConnIDPtr* p = boost::any_cast<ConnIDPtr*>(conn->getContext());
	if (p != 0) {
		(*p)->setId(id);
	}

}
inline uint TMP_ID_CONN(const TcpConnectionPtr& conn) {
	ConnIDPtr* p = boost::any_cast<ConnIDPtr*>(conn->getContext());
	if (p != 0) {
		return (*p)->getTmpId();
	}
	return DEFAULT_ID;
}
inline uint ID_CONN(const muduo::net::TcpConnectionPtr& conn) {
	ConnIDPtr* p = boost::any_cast<ConnIDPtr*>(conn->getContext());
	if (p != 0) {
		return (*p)->getId();
	}
	return DEFAULT_ID;
}
inline std::string TYPE_CONN(const muduo::net::TcpConnectionPtr& conn) {
	ConnIDPtr* p = boost::any_cast<ConnIDPtr*>(conn->getContext());
	if (p != 0) {
		return (*p)->getNodeType();
	}
	return "";;
}
}

#endif /* CONNID_H_ */
