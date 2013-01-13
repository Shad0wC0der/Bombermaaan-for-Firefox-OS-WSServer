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
#include "RequestFactory.h"

class Game : boost::noncopyable{
public:
	enum PLAYER_COLOR{NONE,YELLOW,RED};
	static const short MAX_PLAYER=4; 
	Game(/*std::string,*/Player*,unsigned int);
	virtual ~Game();
	bool addPlayer(Player*);
	bool tryToRemovePlayerByCon(const websocketpp::server::connection_ptr&);
	//std::string getName()const{return this->name;}
	unsigned long getID()const{return this->id;}
	Player* getHost(){return this->host;}
	struct InGamePlayerData{
		PLAYER_COLOR color;
	};
	unsigned short getNbPlayers(){return this->inGamePlayers.size();}
	std::map<Player*,InGamePlayerData> getInGamePlayers(){return inGamePlayers;}
private:
	unsigned int						id;
	std::map<Player*,InGamePlayerData>	inGamePlayers;
	//std::string							name;
	Player*								host;
	boost::mutex						lockInGameMessages;
	
};

#endif /* GAME_H_ */
