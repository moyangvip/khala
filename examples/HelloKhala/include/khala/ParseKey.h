/*
 * ParseKey.h
 *
 *  Created on: Nov 16, 2015
 *      Author: moss
 */

#ifndef PARSEKEY_H_
#define PARSEKEY_H_
namespace khala {
//this is msg parameter
#define MSG_TYPE "type"
#define NODE_TYPE "node_type"
#define DATA "data"
#define RESULT "result"

//this is my msg parameter
#define KEY_ID "id"
#define KEY_PASSWD "passwd"
#define KEY_LOGIN_NAME "name"
#define KEY_FRIEND_ID "friend_id"
#define CHAT_MSG "chatMsg"
#define SEND_NAME "sendName"
#define FRIEND_NAME "friendName"

//this is default msg type
#define LOGIN_TYPE "login"
#define LOGOUT_TYPE "logout"
#define DEV_TYPE "dev"
#define IS_LOGIN_TYPE "isLogin"
#define NODE_ID_TYPE "nodeId"
#define OVER_TIME "overTime"
#define ERR_RUN "errRun"
#define ERR_TYPE "errType"

//this is my msg type
#define TIME_TYPE "time"
#define MY_EVENT "myEvent"
#define ADD_FRIEND "addFriend"
#define GET_FRIENDS "getFriends"
#define CHAT_FRIENDS "chatFriends"
#define CHAT_SEND "chatSend"
#define CHAT_REV "chatRev"

#define ID_BY_TYPE  "idByType"
#define ID_BY_OBJECT "idByObject"
#define ALL_ID_BY_OBJECT "allIdByObject"
#define ALL_ID_BY_OBJECT_PTR "allIdByObjectPtr"
#define ASK_LOGIN "askLogin"
#define SEND_MSG "sendMsg"

//this is default node type
#define TEMP_NODE_TYPE "temp_type"
#define LOGIN_NODE_TYPE "login_type"

//this is my node type
#define MY_NODE_TYPE "my_node_type"
#define MY_USR_TYPE "my_usr_type"
#define MY_CHAT_TYPE "my_chat_type"
#define MY_MANAGE_TYPE "my_manage_type"
}

#endif /* PARSEKEY_H_ */
