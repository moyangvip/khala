/*
 * RandomTempID.h
 *
 *  Created on: Dec 3, 2015
 *      Author: moss
 */

#ifndef COLLECTION_RANDOMONLYID_H_
#define COLLECTION_RANDOMONLYID_H_
#include <cstdlib>
#include <ctime>
#include <boost/unordered_set.hpp>
#include <khala/Keywords.h>
#include <muduo/base/Mutex.h>
namespace khala {
#define Random() (rand()%((uint) 0-1))
class RandomOnlyID {
private:
	RandomOnlyID() {
		idSet_.insert(DEFAULT_ID);
	}
	RandomOnlyID(const RandomOnlyID&);
	RandomOnlyID& operator =(const RandomOnlyID&);
public:
	static RandomOnlyID& getInstance() {
		return randomOnlyID_;
	}
	uint getOnlyID();
	void releaseID(uint);
private:
	typedef boost::unordered_set<uint> IDSet;
	mutable muduo::MutexLock mutex_;
	IDSet idSet_;
	static RandomOnlyID randomOnlyID_;
};
}

#endif /* COLLECTION_RANDOMONLYID_H_ */
