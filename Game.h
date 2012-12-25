/*
 * Game.h
 *
 *  Created on: 9 déc. 2012
 *      Author: robin
 */

#ifndef GAME_H_
#define GAME_H_
#include <websocketpp/websocketpp.hpp>
#include <map>
#include <list>
#include "Player.h"

class Game {
public:
	Game(std::string id);
	virtual ~Game();
	bool addPlayer(Player*);
	bool tryToRemovePlayerByCon(websocketpp::server::connection_ptr); //return Player's con index, -1 if not exit 
private:
	std::string id;
	std::list<Player> inGamePlayers;
	boost::mutex	  lockInGamers;
};

#endif /* GAME_H_ */
