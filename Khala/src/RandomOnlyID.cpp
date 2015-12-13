/*
 * RandomTempID.cpp
 *
 *  Created on: Dec 3, 2015
 *      Author: moss
 */


#include <khala/RandomOnlyID.h>
using namespace khala;
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
