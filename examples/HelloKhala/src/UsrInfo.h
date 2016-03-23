/*
 * UsrInfo.h
 *
 *  Created on: 2015年12月19日
 *      Author: moss
 */

#ifndef USRINFO_H_
#define USRINFO_H_
#include  <set>
#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>

class UsrInfo {
public:
	UsrInfo(const std::string& name);
	virtual ~UsrInfo();
	void addFriends(uint id);
	std::vector<uint> getAllFriends();

	const std::string& getName() const;

private:
	std::string name_;
	std::set<uint> myFriends_;
};
typedef boost::shared_ptr<UsrInfo> UsrInfoPtr;
#endif /* USRINFO_H_ */
