/*
 * LinkedHashSet.h
 *
 *  Created on: Nov 14, 2015
 *      Author: root
 */

#ifndef COLLECTION_LINKEDHASHSET_H_
#define COLLECTION_LINKEDHASHSET_H_
#include <khala/LinkedHashMap.h>
namespace moss {
template<class T1>
class LinkedHashSet {
public:
	LinkedHashSet() {
	}
	bool push_front(T1 value) {
		return linkedHashMap_.push_front(value,value);
	}
	bool find(T1 value) {
		T1* p = linkedHashMap_.find(value);
		return p == 0 ? false : true;
	}
	void remove(T1 value) {
		linkedHashMap_.remove(value);
	}
	T1* back() {
		return linkedHashMap_.back();
	}
	void pop_back() {
		linkedHashMap_.back();
	}
private:
	LinkedHashMap<T1, T1> linkedHashMap_;
};

} /* namespace moss */

#endif /* COLLECTION_LINKEDHASHSET_H_ */
