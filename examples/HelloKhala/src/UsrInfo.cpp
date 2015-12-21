/*
 * UsrInfo.cpp
 *
 *  Created on: 2015年12月19日
 *      Author: root
 */

#include "UsrInfo.h"
#include <muduo/base/Logging.h>
UsrInfo::UsrInfo(const std::string& name) :
		name_(name) {
}
UsrInfo::~UsrInfo() {
	LOG_INFO << "UsrInfo name:" << name_ << " is destructed!";
}
void UsrInfo::addFriends(uint id) {
	myFriends_.insert(id);
}
std::vector<uint> UsrInfo::getAllFriends() {
	std::vector<uint> friends;
	friends.assign(myFriends_.begin(), myFriends_.end());
	return friends;
}

const std::string& UsrInfo::getName() const {
	return name_;
}
