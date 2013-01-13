/*
 * RequestJoinGame.h
 *
 *  Created on: 16 déc. 2012
 *      Author: root
 */

#ifndef REQUESTJOINGAME_H_
#define REQUESTJOINGAME_H_
#include "RequestFactory.h"
#include "Request.h"
#include "Game.h"
#include "JsonParser.h"

class WSServer;
class RequestJoinGame : public Request{
public:
	RequestJoinGame(JsonParser* parser,const websocketpp::server::handler::connection_ptr& con,WSServer* server) : Request(con),server(server){}
	void process(void);
	virtual ~RequestJoinGame(){delete parser;}
private:
	WSServer* server;
	JsonParser* parser;
};

#endif /* REQUESTJOINGAME_H_ */
