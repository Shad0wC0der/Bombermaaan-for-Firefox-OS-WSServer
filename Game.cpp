/*
 * Game.cpp
 *
 *  Created on: 9 déc. 2012
 *      Author: robin
 */

#include "Game.h"

const unsigned short Game::NB_COLORS = 3;

Game::Game(/*std::string name,*/Player* host,const unsigned short& id) {
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

bool Game::isColorAvalaible(PLAYER_COLOR color){
	if(color < NB_COLORS)
		return false;

	for (std::pair<Player*,InGamePlayerData> p : this->getInGamePlayers()){
		if(p.second.color == color)
			return false;
	}

	return true;
}

bool Game::isInGame(const websocketpp::server::connection_ptr& con){
	for (std::pair<Player*,InGamePlayerData> p : this->getInGamePlayers()){
		if(p.first->getCon()==con)
			return true;
	}

	return false;
}

Game::PLAYER_COLOR Game::getColor(const unsigned short& color){
	switch(color){
	case 1:
		return PLAYER_COLOR::YELLOW;break;
	case 2:
		return PLAYER_COLOR::RED;break;
	default:
		return PLAYER_COLOR::NONE;break;
	}
}


