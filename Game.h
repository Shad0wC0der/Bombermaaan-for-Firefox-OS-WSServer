/*
 * Game.h
 *
 *  Created on: 9 déc. 2012
 *      Author: robin
 */

#ifndef GAME_H_
#define GAME_H_

class Game {
public:
	Game(std::string id);
	virtual ~Game();
	bool addPlayer(Player*);
private:
	std::string id;
	std::map<websocketpp::server::handler::connection_ptr,std::string> InGameconnections;
};

#endif /* GAME_H_ */
