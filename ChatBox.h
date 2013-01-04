/*
 * ChatBox.h
 *
 *  Created on: 9 déc. 2012
 *      Author: robin
 */

#ifndef CHATBOX_H_
#define CHATBOX_H_

#include <list>
#include <functional>
#include "Message.h"

class ChatBox {
public:
	static const short MAX_MESSAGE = 20;
	short nbMessage;
	ChatBox();
	virtual ~ChatBox();
	std::list<Message*> getMessages();
	std::function<void(Message*)> addMessage;

private:
	Message* first;
	Message* last;
	void addMessageLessThanMax(Message*);
	void addMessageMoreThanMax(Message*);
};

#endif /* CHATBOX_H_ */
