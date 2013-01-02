/*
 * Game.cpp
 *
 *  Created on: 9 déc. 2012
 *      Author: robin
 */

#include "Game.h"
unsigned long Game::ids = 0;

Game::Game(/*std::string name,*/Player* host) {
	this->id = Game::ids++;
	//this->name=name;
	this->host=host;
	this->addPlayer(host);
}

bool Game::tryToRemovePlayerByCon(websocketpp::server::connection_ptr con){
	for (std::map<Player*,InGamePlayerData>::iterator it = inGamePlayers.begin(); it!=inGamePlayers.end();){
		if(it->first->getCon() == con){
			inGamePlayers.erase(it);
			it->first->~Player();
			return true;
		}else ++it;
	}
	return false;
}

Game::~Game() {
	// TODO Auto-generated destructor stub
}

bool Game::addPlayer(Player* player){
	if(this->inGamePlayers.size()<MAX_PLAYER){
		InGamePlayerData inGamePlayerData;
		inGamePlayerData.color=NONE;
		this->inGamePlayers.insert(std::pair<Player*,InGamePlayerData>(player,inGamePlayerData));
		return true;
	}
	return false;
}

