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
#define stringify( name ) # name
class WSServer;
class RequestMessage : public Request{
public:
	enum MESSAGE_TYPE {TO_ALL,
					   TO_ROOM,
					   TO_PLAYER};

	RequestMessage(JsonParser* parser,const websocketpp::server::handler::connection_ptr& con,WSServer* server) : Request(con),parser(parser),server(server){}
	void process(void);
	virtual ~RequestMessage(){delete parser;}
private:
	 JsonParser* parser;
	 WSServer* server;
};

#endif /* REQUESTMESSAGE_H_ */
