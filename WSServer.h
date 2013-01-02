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
void tickInGame();
class WSServer : public websocketpp::server::handler {
public:
	static const int IN_GAME_TICK = 80;
	void on_open(connection_ptr con);
	void on_message(connection_ptr connection,message_ptr msg);
	void on_close(connection_ptr con);
	std::string get_con_id(connection_ptr con);
	WSServer();
	virtual ~WSServer();
	RequestCoordinator* getCoordinator(){return &coordinator;}
	unsigned long addNewGame(Player*);
	void removeGame(Game*);
	void addNewPlayer(connection_ptr,std::string);
	std::list<Game>* getGames(){return &games;}
	Player* takeOutGamePlayerByCon(connection_ptr);
	void notifyPlayerJoined(std::string,std::string);
	void notifyMessageSent(std::string,std::string,std::string);
	void notifyGameCreated(std::string,std::string);
private:
	std::list<Player*>	outGamePlayers;
	std::list<Game> 	games;
	RequestCoordinator  coordinator;
	boost::mutex 		lockOutGamers;
	boost::mutex		lockInGamers;
	RequestFactory*		requestFactory;
};

#endif /* WSSERVER_H_ */
