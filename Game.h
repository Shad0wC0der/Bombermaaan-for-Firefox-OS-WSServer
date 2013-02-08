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
#include <utility>
#include "Map.h"
#include "Player.h"
#include "RequestFactory.h"

class Game : boost::noncopyable{
public:
	enum PLAYER_COLOR{NONE,YELLOW,RED};
	static const unsigned short NB_COLORS;
	static const short MAX_PLAYER=4; 
	Game(/*std::string,*/Player*,const unsigned short&);
	virtual ~Game();
	bool addPlayer(Player*);
	bool tryToRemovePlayerByCon(const websocketpp::server::connection_ptr&);
	//std::string getName()const{return this->name;}
	unsigned short getID()const{return this->id;}
	Player* getHost(){return this->host;}
	struct InGamePlayerData{
		PLAYER_COLOR color;
	};
	unsigned short getNbPlayers(){return this->inGamePlayers.size();}
	std::map<Player*,InGamePlayerData> getInGamePlayers(){return inGamePlayers;}
	Map* getMap(){return &map;}
	bool isColorAvalaible(PLAYER_COLOR);
	bool isInGame(const websocketpp::server::connection_ptr&);
	PLAYER_COLOR getColor(const unsigned short&);
private:
	unsigned short						id;
	std::map<Player*,InGamePlayerData>	inGamePlayers;
	//std::string							name;
	Player*								host;
	boost::mutex						lockInGameMessages;
	Map									map;
	
};

#endif /* GAME_H_ */
