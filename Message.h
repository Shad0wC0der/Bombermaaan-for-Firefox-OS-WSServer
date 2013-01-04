/*
 * Message.h
 *
 *  Created on: 9 déc. 2012
 *      Author: robin
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <string>

class Message {
public:
	Message(std::string,std::string,std::string);
	virtual ~Message();
	void setNext(Message* next){this->next=next;}
	Message* getNext(){return next;}
private:
	Message* next;
	std::string message;
	std::string authorName;
	std::string authorID;
};

#endif /* MESSAGE_H_ */
