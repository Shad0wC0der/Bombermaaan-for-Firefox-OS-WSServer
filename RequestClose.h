/*
 * RequestRefreshOutGameData.h
 *
 *  Created on: 16 déc. 2012
 *      Author: root
 */

#ifndef REQUESTCLOSE_H_
#define REQUESTCLOSE_H_
#include "RequestFactory.h"
#include "Request.h"
#include "Game.h"

class WSServer;
class RequestClose : public Request{
public:
	RequestClose(const websocketpp::server::handler::connection_ptr& con,WSServer* server) : Request(con),server(server){}
	void process(void);
	virtual ~RequestClose(){}
private:
	WSServer* server;
};

#endif /* REQUESTCLOSE_H_ */
