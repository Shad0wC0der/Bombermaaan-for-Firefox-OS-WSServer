/*
 * Message.h
 *
 *  Created on: 9 déc. 2012
 *      Author: robin
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <string>
#include <time.h>
#include <sstream>

class Message {
public:
	Message(const std::string&, const std::string&, const std::string&);
	virtual ~Message();
	void setNext(Message* next){this->next=next;}
	Message* getNext(){return next;}

	std::string getMessage()const{return message;}
	std::string getAuthorID()const{return authorID;}
	std::string getAuthorName()const{return authorName;}
	std::string getSubmitTime()const{std::ostringstream oss; oss<<this->submitTime;return oss.str();}
private:
	Message* next;
	std::string message;
	std::string authorName;
	std::string authorID;
	time_t submitTime;
};

#endif /* MESSAGE_H_ */
