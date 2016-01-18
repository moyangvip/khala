/*
 * ConnID.cpp
 *
 *  Created on: Dec 6, 2015
 *      Author: moss
 */
#include <khala/ConnID.h>
using namespace khala;
ConnID::ConnID() :
		tmpId_(DEFAULT_ID), id_(DEFAULT_ID) {
}
uint ConnID::getId() const {
	return id_;
}

void ConnID::setId(uint id) {
	this->id_ = id;
}

uint ConnID::getTmpId() const {
	return tmpId_;
}

void ConnID::setTmpId(uint tmpId) {
	this->tmpId_ = tmpId;
}
const std::string& ConnID::getNodeType() const{
	return objectType_;
}
void ConnID::setNodeType(const std::string& objectType){
	this->objectType_ = objectType;
}
