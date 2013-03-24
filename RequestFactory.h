/*
 * RequestFactory.h
 *
 *  Created on: 16 déc. 2012
 *      Author: root
 */

#ifndef REQUESTFACTORY_H_
#define REQUESTFACTORY_H_
#include "vjson/json.h"
#include "Request.h"
#include "JsonParser.h"
#define stringify( name ) # name
class WSServer;

class RequestFactory {
public:
	enum REQUEST_TYPE {
					   /* out-room requests */
					   MESSAGE,
					   REFRESH_OUT_GAME_DATA,
					   SEND_SELF_DATA,
					   CREATE_GAME,
					   JOIN_GAME,
					   NOTIFY_PLAYER_JOINED,
					   NOTIFY_PLAYER_EXITED,
					   NOTIFY_MESSAGE_SENT,
					   NOTIFY_GAME_CREATED,
					   NOTYFY_GAME_REMOVED,
					   NOTIFY_ERROR,
					   NOTIFY_ENTERING_ROOM,
					   NOTIFY_EXITING_ROOM,
					   /* in-room requests */
					   GAME_MAP_LIST,
					   GAME_MAP_DATA,
					   LAUNCH_GAME,
					   NOTIFY_COLOR_CHANGED,
					   SELECT_SERVER_MAP,
					   CHOOSE_COLOR,
					   NOTIFY_GAME_STARTED,
					   NOTIFY_GAME_FINISHED,
					   /* in-game requests */
					   MOVE,
					   DROP_BOMB,
					   NOTIFY_MOVE,
					   NOTIFY_BOMB_DROPED,
					   NOTIFY_BOMB_EXPLODED,
					   NOTIFY_PLAYER_KILLED,
					   NOTIFY_BLOCK_DESTROYED,
					   NOTIFY_ITEM_APPEARED,
					   NOTIFY_ITEM_DESTROYED,
					   NOTIFY_ITEM_PICKEDUP,
					   NOTIFY_BONUS_ACQUIRED
					  };

	RequestFactory(WSServer* server);
	virtual ~RequestFactory(){}
	Request* createRequest(const websocketpp::server::handler::connection_ptr&,const std::string&);
private:
	WSServer* server;
};

#endif /* REQUESTFACTORY_H_ */
