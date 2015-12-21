/*
 * RandomTempID.cpp
 *
 *  Created on: Dec 3, 2015
 *      Author: moss
 */

#include <khala/RandomOnlyID.h>
#include <ctime>
#include <cstdlib>
using namespace khala;
#define Random() (rand()%((uint) 0-1))
RandomOnlyID RandomOnlyID::randomOnlyID_;

uint RandomOnlyID::getOnlyID() {
	srand(time(0));
	while (true) {
		uint newId = Random();
		{
			muduo::MutexLockGuard lock(mutex_);
			typename IDSet::iterator it = idSet_.find(newId);
			if (it == idSet_.end()) {
				idSet_.insert(newId);
				return newId;
			}
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
RandomOnlyID::RandomOnlyID() {
	idSet_.insert(DEFAULT_ID);
}
RandomOnlyID& RandomOnlyID::getInstance() {
	return randomOnlyID_;
}
