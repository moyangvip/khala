//============================================================================
// Name        : HelloKhala.cpp
// Author      : Moss
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <khala/NodeServer.h>
using namespace khala;
int main() {
	InetAddress listenAddr(USER_PORT);
	NodeServer nodeServer(listenAddr);
	nodeServer.start(USER_SERVER_THREAD_NUM);
	return 0;
}
