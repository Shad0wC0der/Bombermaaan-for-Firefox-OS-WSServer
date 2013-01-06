/*
 * RequestRefreshOutGameData.h
 *
 *  Created on: 16 déc. 2012
 *      Author: root
 */

#ifndef REQUESTREFRESHOUTGAMEDATA_H_
#define REQUESTREFRESHOUTGAMEDATA_H_
#include "RequestFactory.h"
#include "Request.h"
#include "Game.h"

class WSServer;
class RequestRefreshOutGameData : public Request{
public:
	RequestRefreshOutGameData(websocketpp::server::handler::connection_ptr con,WSServer* server) : Request(con),server(server){}
	void process(void);
	virtual ~RequestRefreshOutGameData(){}
private:
	WSServer* server;
};

#endif /* REQUESTREFRESHOUTGAMEDATA_H_ */
