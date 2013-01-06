/*
 * ChatBox.cpp
 *
 *  Created on: 9 déc. 2012
 *      Author: robin
 */

#include "ChatBox.h"

ChatBox::ChatBox() {
	this->nbMessage=0;
	this->first=(Message*)0;
	this->last=(Message*)0;
	this->addMessage=std::bind(&ChatBox::addMessageLessThanMax, this, std::placeholders::_1);
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
		addMessage=std::bind(&ChatBox::addMessageMoreThanMax, this, std::placeholders::_1);
}

void ChatBox::addMessageMoreThanMax(Message* message){
	this->last->setNext(message);
	this->last=message;
	Message * m = this->first->getNext();
	delete this->first;
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