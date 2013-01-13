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

class WSServer;
class RequestCreateGame : public Request{
public:
	RequestCreateGame(const websocketpp::server::handler::connection_ptr& con,WSServer* server) : Request(con),server(server){}
	void process(void);
	virtual ~RequestCreateGame(){}
private:
	WSServer* server;
};

#endif /* REQUESTCREATEGAME_H_ */
