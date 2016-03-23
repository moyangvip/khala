//============================================================================
// Name        : HelloKhala.cpp
// Author      : Moss
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <khala/NodeServer.h>
#include "ChatType.h"
#include "MyManageType.h"
#include "MyNodeType.h"
#include "MyUsrType.h"
#include "MyTempNodeType.h"
using namespace khala;
int main() {
	InetAddress listenAddr(USER_PORT);
	NodeServer nodeServer(listenAddr);
	/*
	ChatType chatType;
	nodeServer.addNodeType(&chatType);

	MyManageType myManageType;
	nodeServer.addNodeType(&myManageType);

	MyNodeType myNodeType;
	nodeServer.addNodeType(&myNodeType);

	MyUsrType myUsrType;
	nodeServer.addNodeType(&myUsrType);

	MyTempNodeType myTempNodeType;
	nodeServer.setTempNodeType(&myTempNodeType);
	*/
	nodeServer.start(USER_SERVER_THREAD_NUM);
	return 0;
}
