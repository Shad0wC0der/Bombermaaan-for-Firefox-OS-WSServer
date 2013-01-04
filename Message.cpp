/*
 * Message.cpp
 *
 *  Created on: 9 déc. 2012
 *      Author: robin
 */

#include "Message.h"

Message::Message(std::string message,std::string authorID,std::string authorName) {
	this->message=message;
	this->authorName=authorName;
	this->authorID=authorID;
}

Message::~Message() {
	// TODO Auto-generated destructor stub
}
