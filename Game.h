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
	enum PLAYER_COLOR{NONE,YELLOW,RED};
	static const short MAX_PLAYER=4; 
	Game(/*std::string,*/Player*);
	virtual ~Game();
	bool addPlayer(Player*);
	bool tryToRemovePlayerByCon(websocketpp::server::connection_ptr);
	//std::string getName()const{return this->name;}
	unsigned long getID()const{return this->id;}
	struct InGamePlayerData{
		PLAYER_COLOR color;
	};
	static unsigned long ids;
private:
	unsigned long						id;
	std::map<Player*,InGamePlayerData>	inGamePlayers;
	//std::string							name;
	Player*								host;
	
};

#endif /* GAME_H_ */
