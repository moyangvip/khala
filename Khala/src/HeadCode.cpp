/*
 * HeadDispatcher.cpp
 *
 *  Created on: Nov 9, 2015
 *      Author: moss
 */

#include <khala/HeadCode.h>
#include <muduo/base/Logging.h>
using namespace khala;
HeadCode::HeadCode() {

}
std::string HeadCode::onCode(const muduo::net::TcpConnectionPtr& conn,
		                 muduo::net::Buffer* buf,
		                 muduo::Timestamp time){
	std::string message;
	while (buf->readableBytes() >= kHeaderLen) // kHeaderLen == 4
	{
		// FIXME: use Buffer::peekInt32()
		const void* data = buf->peek();
		int32_t be32 = *static_cast<const int32_t*>(data); // SIGBUS
		const int32_t len = muduo::net::sockets::networkToHost32(be32);
		if (len > 65536 || len < 0) {
			LOG_ERROR << "Invalid length " << len;
			conn->shutdown();  // FIXME: disable reading
			break;
		} else if (buf->readableBytes() >= len + kHeaderLen) {
			buf->retrieve(kHeaderLen);
			message.assign(buf->peek(), buf->peek()+len);
			buf->retrieve(len);
		} else {
			break;
		}
	}
	return message;
}
