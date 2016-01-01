/*
 * HeadDispatcher.h
 *
 *  Created on: Nov 9, 2015
 *      Author: moss
 */

#ifndef HEADCODE_H_
#define HEADCODE_H_
#include <muduo/net/TcpConnection.h>
namespace khala {
class HeadCode {
private:
	HeadCode();
	HeadCode(const HeadCode&);
	HeadCode& operator = (const HeadCode&);
public:
	static HeadCode* getInstance();
	std::string onCode(const muduo::net::TcpConnectionPtr& conn,
			muduo::net::Buffer* buf, muduo::Timestamp time);
	muduo::net::Buffer addHeadCode(const std::string& msg);
private:
	const static unsigned int kHeaderLen = sizeof(int32_t);
	static HeadCode headCode_;
};
}

#endif /* HEADCODE_H_ */
