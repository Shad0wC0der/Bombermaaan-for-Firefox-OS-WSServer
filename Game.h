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

enum DIRECTION {
				UP,
				RIGHT,
				DOWN,
				LEFT
				};

enum ITEM_TYPE {
				BOOT,
				POWER,
				BOMB
				};

enum PLAYER_COLOR{
				NONE,
				WHITE,
				BLACK,
				BLUE,
				RED
				};

struct Move{
	DIRECTION direction;
	unsigned short int timer;
	unsigned short int endTime;
	unsigned short int tChangePosition;
};

struct Bomb{
	unsigned short int radius;
	unsigned short int timer;
	unsigned short int endTime;
	Position position;
};

struct Item{
	ITEM_TYPE type;
	Position position;
};

class Game : boost::noncopyable{
public:
	static const unsigned short NB_COLORS;
	static const unsigned short MAX_PLAYER=4; 
	static const unsigned short BOMB_TIMER=120;
	static const unsigned short DEFLAGRATION_TIMER=10;
	Game(/*std::string,*/Player*,const unsigned short&,WSServer*);
	virtual ~Game();
	bool addPlayer(Player*);
	bool tryToRemovePlayerByCon(const websocketpp::server::connection_ptr&);
	//std::string getName()const{return this->name;}
	unsigned short getID()const{return this->id;}
	Player* getHost(){return this->host;}
	struct InGamePlayerData{
		PLAYER_COLOR color;
		Position position;
		unsigned short speed;
		unsigned short radius;
		unsigned short bombUsed;
		unsigned short maxBomb;
	};
	unsigned short getNbPlayers(){return this->nbPlayers;}
	Player* getPlayer(const unsigned short&);
	Map* getMap(){return &map;}
	bool isColorAvalaible(const unsigned short&);
	bool isInGame(const websocketpp::server::connection_ptr&);
	PLAYER_COLOR getColor(const unsigned short&);
	void startGame();
	void dropBomb(const unsigned short&,const websocketpp::server::connection_ptr&);
	void move(const unsigned short&,const websocketpp::server::connection_ptr&,const unsigned short&);
	void changePlayerColor(const websocketpp::server::connection_ptr&,const unsigned short&);
	void perform();

	void notifyEnteringRoom(Player* player);
	void notifyExitingRoom(const std::string& playerID);
	void notifyGameStarted();
	void notifyGameFinished();
	void notifyColorChanged();
	void notifyMove(const unsigned short&,const Move&);
	void notifyBombDropped(const unsigned short&,const Bomb&);

private:
	unsigned short						id;
	std::pair<Player*,InGamePlayerData>*inGamePlayers;
	unsigned short						nbPlayers;
	//std::string							name;
	Player*								host;
	boost::mutex						lockInGameMessages;
	Map									map;
	Move								moves[MAX_PLAYER];
	std::list<Bomb>						bombs;
	std::list<Item>						items;
	bool**								blocks;
	bool**								bombObstructions;
	unsigned short int**				deflagrations;
	WSServer*							server;
	bool								isObstructed(const unsigned short&,const unsigned short&);

};

#endif /* GAME_H_ */
