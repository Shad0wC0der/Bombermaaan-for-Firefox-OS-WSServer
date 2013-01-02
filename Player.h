/*
 * Player.h
 *
 *  Created on: 9 déc. 2012
 *      Author: robin
 */

#ifndef PLAYER_H_
#define PLAYER_H_
#include <string>
#include <websocketpp/websocketpp.hpp>

class Player {
private:
	std::string							name;
	websocketpp::server::connection_ptr con;
	std::string							id;
public:
	Player(websocketpp::server::connection_ptr,std::string,std::string);
	virtual ~Player();
	std::string getName() const;
	void setname(std::string);
	websocketpp::server::connection_ptr getCon() const; 
};

#endif /* PLAYER_H_ */
