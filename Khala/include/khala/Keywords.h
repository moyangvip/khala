/*
 * Keywords.h
 *
 *  Created on: Nov 17, 2015
 *      Author: moss
 */

#ifndef KEYWORDS_H_
#define KEYWORDS_H_
#include <muduo/net/TcpServer.h>
namespace khala {
//never use this id as node key
#define DEFAULT_ID 0

typedef unsigned int uint;
typedef muduo::net::TcpConnectionPtr TcpConnectionPtr;
typedef muduo::Timestamp Timestamp;
typedef muduo::net::InetAddress InetAddress;
}

#endif /* KEYWORDS_H_ */
