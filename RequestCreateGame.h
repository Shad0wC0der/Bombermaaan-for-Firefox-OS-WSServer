/*
 * RequestCreateGame.h
 *
 *  Created on: 16 déc. 2012
 *      Author: root
 */

#ifndef REQUESTCREATEGAME_H_
#define REQUESTCREATEGAME_H_
#include "RequestFactory.h"
#include "Request.h"
#include "Game.h"
#include "JsonParser.h"

class WSServer;
class RequestCreateGame : public Request{
public:
	RequestCreateGame(JsonParser* parser,websocketpp::server::handler::connection_ptr con,WSServer* server) : Request(con),parser(parser),server(server){}
	void process(void);
	virtual ~RequestCreateGame(){delete parser;}
private:
	WSServer* server;
	JsonParser* parser;
};

#endif /* REQUESTCREATEGAME_H_ */
