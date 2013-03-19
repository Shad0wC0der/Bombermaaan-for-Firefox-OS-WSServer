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
	Player(const websocketpp::server::connection_ptr&,const std::string&,const std::string&);
	virtual ~Player(){}
	std::string getName() const{return this->name;}
	void setName(std::string name){this->name=name;}
	websocketpp::server::connection_ptr getCon()const{return this->con;}
	std::string getID()const{return id;}
};

#endif /* PLAYER_H_ */
