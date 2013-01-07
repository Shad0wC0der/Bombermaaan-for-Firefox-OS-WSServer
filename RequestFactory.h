/*
 * RequestFactory.h
 *
 *  Created on: 16 déc. 2012
 *      Author: root
 */

#ifndef REQUESTFACTORY_H_
#define REQUESTFACTORY_H_
#include "vjson/json.h"
#include "RequestMessage.h"
#include "RequestRefreshOutGameData.h"
#include "RequestSendSelfData.h"
#include "Request.h"
#include "JsonParser.h"
#define stringify( name ) # name
class WSServer;

class RequestFactory {
public:
	enum REQUEST_TYPE {MESSAGE,
					   REFRESH_OUT_GAME_DATA,
					   SEND_SELF_DATA,
					   CREATE_GAME,
					   JOIN_GAME,
					   NOTIFY_PLAYER_JOINED,
					   NOTIFY_PLAYER_EXITED,
					   NOTIFY_MESSAGE_SENT,
					   NOTIFY_GAME_CREATED};

	RequestFactory(WSServer* server);
	virtual ~RequestFactory(){}
	Request* createRequest(websocketpp::server::handler::connection_ptr,std::string);
private:
	WSServer* server;
};

#endif /* REQUESTFACTORY_H_ */
