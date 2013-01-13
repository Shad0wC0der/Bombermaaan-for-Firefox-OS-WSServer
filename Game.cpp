/*
 * Game.cpp
 *
 *  Created on: 9 déc. 2012
 *      Author: robin
 */

#include "Game.h"

Game::Game(/*std::string name,*/Player* host,unsigned int id) {
	this->id = id;
	//this->name=name;
	this->host=host;
	this->addPlayer(host);
}

bool Game::tryToRemovePlayerByCon(const websocketpp::server::connection_ptr& con){
	bool hostExited=false;
	for (std::map<Player*,InGamePlayerData>::iterator it = inGamePlayers.begin(); it!=inGamePlayers.end();){
		if(it->first->getCon() == con){
			if(this->host == it->first)hostExited=true;
			delete it->first;
			inGamePlayers.erase(it);
			if(hostExited && !inGamePlayers.empty())
				host=inGamePlayers.begin()->first;
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

