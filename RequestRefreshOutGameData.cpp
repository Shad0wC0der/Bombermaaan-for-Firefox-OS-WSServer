/*
 * RequestRefreshOutGameData.cpp
 *
 *  Created on: 16 déc. 2012
 *      Author: root
 */

#include "RequestRefreshOutGameData.h"
#include "WSServer.h"

void RequestRefreshOutGameData::process(void){
	std::string response="{\"type\":\""+std::string(stringify(REFRESH_OUT_GAME_DATA))+"\",\"value\":{\"games\":[";
	if(!(this->server->getGames()->empty())){
		std::list<Game>::iterator it = this->server->getGames()->begin();
		std::ostringstream oss;
		oss<<it->getID();
		std::string id=oss.str();
		response+="\""+id+"\"";
		++it;
		while(it != this->server->getGames()->end()){
			std::ostringstream oss;
			oss<<it->getID();
			std::string id=oss.str();
			response+=",\""+id+"\"";
			++it;
		}
	}
	response+="],\"players\":[";
	if(!(this->server->getOutGamePlayers()->empty())){
		std::list<Player*>::iterator it = this->server->getOutGamePlayers()->begin();
		std::ostringstream oss;
		oss<<"{\"name\":\""<<(*it)->getName()<<"\",\"id\":\""<<(*it)->getID()<<"\"}";
		std::string player=oss.str();
		response+=player;
		++it;
		while(it != this->server->getOutGamePlayers()->end()){
			std::list<Player*>::iterator it = this->server->getOutGamePlayers()->begin();
			std::ostringstream oss;
			oss<<",{\"name\":\""<<(*it)->getName()<<"\",\"id\":\""<<(*it)->getID()<<"\"}";
			std::string player=oss.str();
			response+=player;
			++it;
		}
	}

	response+="]}}";
	con->send(response);
}