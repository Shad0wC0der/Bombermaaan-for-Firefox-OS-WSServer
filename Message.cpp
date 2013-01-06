/*
 * Message.cpp
 *
 *  Created on: 9 déc. 2012
 *      Author: robin
 */

#include "Message.h"
Message::Message(const std::string& message, const std::string& authorName, const std::string& authorID) {
	this->message=message;
	this->authorName=authorName;
	this->authorID=authorID;
	this->next=(Message*)0;
	
	this->submitTime=time(NULL);
}

Message::~Message() {
	// TODO Auto-generated destructor stub
}
