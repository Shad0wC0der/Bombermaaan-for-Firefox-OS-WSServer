/*
 * ChatBox.cpp
 *
 *  Created on: 9 déc. 2012
 *      Author: robin
 */

#include "ChatBox.h"

ChatBox::ChatBox() {
	this->nbMessage=0;
	this->addMessage=&ChatBox::addMessageLessThanMax;
}

ChatBox::~ChatBox() {
	// TODO Auto-generated destructor stub
}

void ChatBox::addMessageLessThanMax(Message* message){
	if(this->nbMessage==0){
		this->last=message;
		this->first=message;
	}else{
		this->last->setNext(message);
		this->last=message;
	}

	++this->nbMessage;
	if(nbMessage>=ChatBox::MAX_MESSAGE)
		addMessage=&ChatBox::addMessageMoreThanMax;
}

void ChatBox::addMessageMoreThanMax(Message* message){
	this->last->setNext(message);
	this->last=message;
	Message * m = this->first->getNext();
	this->first->~Message();
	this->first=m;
}

std::list<Message*> ChatBox::getMessages(){
	std::list<Message*> messages;

	Message* m = this->first;
	while(m!=((Message*)0)){
		messages.push_back(m);
		m=m->getNext();
	}

	return messages;
}