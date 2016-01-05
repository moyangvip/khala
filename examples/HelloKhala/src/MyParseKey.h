/*
 * MyParseKey.h
 *
 *  Created on: 2016年1月5日
 *      Author: moss
 */

#ifndef MYPARSEKEY_H_
#define MYPARSEKEY_H_

/*
 * you can set your parse key here
 * default ParseKey at (./include/khala/ParseKey.h)
 * you should avoid define conflicts.
 * */

//this is my msg parameter
#define KEY_ID "id"
#define KEY_PASSWD "passwd"
#define KEY_LOGIN_NAME "name"
#define KEY_FRIEND_ID "friend_id"
#define CHAT_MSG "chatMsg"
#define SEND_NAME "sendName"
#define FRIEND_NAME "friendName"


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


//this is my node type
#define MY_NODE_TYPE "my_node_type"
#define MY_USR_TYPE "my_usr_type"
#define MY_CHAT_TYPE "my_chat_type"
#define MY_MANAGE_TYPE "my_manage_type"



#endif /* MYPARSEKEY_H_ */
