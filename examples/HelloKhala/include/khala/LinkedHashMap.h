/*
 * LinkedHashMap.h
 *
 *  Created on: Nov 14, 2015
 *      Author: root
 */

#ifndef LINKEDHASHMAP_H_
#define LINKEDHASHMAP_H_
#include <list>
#include <boost/unordered_map.hpp>
#include <muduo/base/Mutex.h>

namespace moss {
template<class T1, class T2>
class LinkedHashMap {
public:
	LinkedHashMap() {
	}
	bool push_front(const T1& key, const T2& value) {
		muduo::MutexLockGuard lock(mutex_);
		typename NewMap::iterator mapIt = map_.find(key);
		if (mapIt != map_.end()) {
			//the key is already exists
			return false;
		}
		list_.push_front(std::make_pair(key, value));
		map_.insert(make_pair(key, list_.begin()));
		return true;
	}
	//threading unsafe,because the return pointer is unlock,point object may be delete
	T2* find(const T1& key) {
		muduo::MutexLockGuard lock(mutex_);
		typename NewMap::iterator mapIt = map_.find(key);
		if (mapIt == map_.end()) {
			//the key is not exists
			return 0;
		}
		typename NewList::iterator listIt = mapIt->second;
		return &((*listIt).second);
	}
	void remove(const T1& key) {
		muduo::MutexLockGuard lock(mutex_);
		remove_(key);
	}
	//must check map is not empty
	T2& back() {
		muduo::MutexLockGuard lock(mutex_);
		typename NewList::reference val = list_.back();
		return (val.second);
	}
	void pop_back() {
		muduo::MutexLockGuard lock(mutex_);
		if (!list_.empty()) {
			typename NewList::reference val = list_.back();
			T1 key = val.first;
			remove_(key);
		}
		return;
	}
	void updateKey(const T1& key) {
		muduo::MutexLockGuard lock(mutex_);
		typename NewMap::iterator mapIt = map_.find(key);
		if (mapIt == map_.end()) {
			//the key is not exists
			return;
		}
		typename NewList::iterator listIt = mapIt->second;
		//put value to first
		list_.push_front(*listIt);
		//update map to new list iterator
		map_[key] = list_.begin();
		//delete the old
		list_.erase(listIt);
	}
private:
	void remove_(const T1& key) {
		typename NewMap::iterator mapIt = map_.find(key);
		if (mapIt == map_.end()) {
			return;
		}
		typename NewList::iterator listIt = mapIt->second;
		list_.erase(listIt);
		map_.erase(mapIt);
	}
private:
	typedef std::list<std::pair<T1, T2> > NewList;
	typedef boost::unordered_map<T1, typename NewList::iterator> NewMap;
public:
	typedef typename NewMap::size_type size_type;
	size_type size() {
		return map_.size();
	}
private:
	NewList list_;
	NewMap map_;
	mutable muduo::MutexLock mutex_;
};

} /* namespace moss */

#endif /* LINKEDHASHMAP_H_ */
