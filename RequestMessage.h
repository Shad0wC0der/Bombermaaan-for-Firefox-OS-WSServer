/*
 * RequestMessage.h
 *
 *  Created on: 16 déc. 2012
 *      Author: root
 */

#ifndef REQUESTMESSAGE_H_
#define REQUESTMESSAGE_H_
#include "RequestFactory.h"
#include "Request.h"
#include "JsonParser.h"
class WSServer;
class RequestMessage : public Request{
public:
	RequestMessage(JsonParser* parser,websocketpp::server::handler::connection_ptr con,WSServer* server) : Request(con){this->parser=parser;this->server=server;}
	void process(void);
	virtual ~RequestMessage(){parser->~JsonParser();}
private:
	 JsonParser* parser;
	 WSServer* server;
};

#endif /* REQUESTMESSAGE_H_ */
