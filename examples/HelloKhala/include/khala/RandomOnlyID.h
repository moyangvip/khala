/*
 * RandomTempID.h
 *
 *  Created on: Dec 3, 2015
 *      Author: moss
 */

#ifndef COLLECTION_RANDOMONLYID_H_
#define COLLECTION_RANDOMONLYID_H_
#include <khala/Keywords.h>
#include <boost/unordered_set.hpp>
#include <muduo/base/Mutex.h>
namespace khala {
class RandomOnlyID {
private:
	RandomOnlyID();
	RandomOnlyID(const RandomOnlyID&);
	RandomOnlyID& operator =(const RandomOnlyID&);
public:
	static RandomOnlyID& getInstance();
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
