/*
 * Player.cpp
 *
 *  Created on: 9 déc. 2012
 *      Author: robin
 */

#include "Player.h"

Player::Player(websocketpp::server::connection_ptr con,std::string id) {
	this->con = con;
	this->id = id;
}

Player::~Player() {
	// TODO Auto-generated destructor stub
}


std::string Player::getName()const{
	return this->name;
}

void Player::setname(std::string){

}

websocketpp::server::connection_ptr Player::getCon()const{
	return this->con;
}
