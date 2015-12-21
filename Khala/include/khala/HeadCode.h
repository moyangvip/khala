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
public:
	HeadCode();
	std::string onCode(const muduo::net::TcpConnectionPtr& conn,
			muduo::net::Buffer* buf, muduo::Timestamp time);
private:
	const static unsigned int kHeaderLen = sizeof(int32_t);
};
}

#endif /* HEADCODE_H_ */
