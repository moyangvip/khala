/*
 * RandomTempID.cpp
 *
 *  Created on: Dec 3, 2015
 *      Author: moss
 */

#include <khala/RandomOnlyID.h>
#include <muduo/base/Logging.h>
using namespace khala;

RandomOnlyID RandomOnlyID::randomOnlyID_;
const uint RandomOnlyID::MAX_ID = 0 - 1;
uint RandomOnlyID::getOnlyID() {
	muduo::MutexLockGuard lock(mutex_);
	while (true) {
		if (currMaxID == MAX_ID) {
			LOG_WARN<<"currMaxID is MAX_ID:"<<MAX_ID;
			currMaxID = 0;
		} else {
			++currMaxID;
		}
		typename IDSet::iterator it = idSet_.find(currMaxID);
		if (it == idSet_.end()) {
			idSet_.insert(currMaxID);
			return currMaxID;
		}
	}
}
void RandomOnlyID::releaseID(uint id) {
	muduo::MutexLockGuard lock(mutex_);
	typename IDSet::iterator it = idSet_.find(id);
	if (it != idSet_.end()) {
		idSet_.erase(it);
	}
}
RandomOnlyID::RandomOnlyID() :
		currMaxID(0) {
	idSet_.insert(DEFAULT_ID);
}
RandomOnlyID& RandomOnlyID::getInstance() {
	return randomOnlyID_;
}
