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
#define KEY_ID "id"
#define KEY_PASSWD "passwd"
#define KEY_LOGIN_NAME "name"
#define NODE_TYPE "node_type"

//this is my msg parameter
#define KEY_FRIEND_ID "friend_id"

//this is default msg type
#define LOGIN_TYPE "login"
#define LOGOUT_TYPE "logout"
#define DEV_TYPE "dev"
#define IS_LOGIN_TYPE "isLogin"
#define NODE_ID_TYPE "nodeId"

//this is my msg type
#define TIME_TYPE "time"
#define MY_EVENT "myEvent"
#define ADD_FRIEND "addFriend"
#define GET_FRIENDS "getFriends"
//#define SUBSCRIBE_TYPE "subscribe"
//#define UNSUBSCRIBE_TYPE "unsubscribe"
//#define AVAILABLE_RES_TYPE "available_res"
//#define SUBSCRIBED_RES_TYPE "subscribed_res"

//this is default node type
#define TEMP_NODE_TYPE "temp_type"
#define LOGIN_NODE_TYPE "login_type"

//this is my node type
#define MY_NODE_TYPE "my_node_type"
#define MY_USR_TYPE "my_usr_type"
}

#endif /* PARSEKEY_H_ */
