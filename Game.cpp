/*
 * Game.cpp
 *
 *  Created on: 9 déc. 2012
 *      Author: robin
 */

#include "Game.h"

Game::Game(std::string id) {
	this->id = id;
}

bool Game::tryToRemovePlayerByCon(websocketpp::server::connection_ptr con){
	boost::unique_lock<boost::mutex> l(lockInGamers);
	for (std::list<Player>::iterator it = inGamePlayers.begin(); it!=inGamePlayers.end();){
		if(it->getCon() == con){
			inGamePlayers.erase(it);
			return true;
		}else ++it;
	}
	return false;
}

Game::~Game() {
	// TODO Auto-generated destructor stub
}

