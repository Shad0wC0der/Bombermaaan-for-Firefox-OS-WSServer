/*
 * WSServer.h
 *
 *  Created on: 9 déc. 2012
 *      Author: robin
 */

#ifndef WSSERVER_H_
#define WSSERVER_H_
#include <sstream>
#include "RequestCoordinator.h"
#include "Game.h"

void process(RequestCoordinator*);
class WSServer : public websocketpp::server::handler {
public:
	void on_open(connection_ptr con);
	void on_message(connection_ptr connection,message_ptr msg);
	void on_close(connection_ptr con);
	std::string get_con_id(connection_ptr con);
	WSServer();
	virtual ~WSServer();
	RequestCoordinator* getCoordinator(){return &coordinator;}
	Game* addNewGame(Player*);
	void removeGame(Game*);
private:
	std::list<Player>	outGamePlayers;
	std::list<Game> 	games;
	RequestCoordinator  coordinator;
	boost::mutex 		lockOutGamers;
	boost::mutex		lockInGamers;
};

#endif /* WSSERVER_H_ */
