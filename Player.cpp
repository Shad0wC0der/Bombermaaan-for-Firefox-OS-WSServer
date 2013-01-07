/*
 * Player.cpp
 *
 *  Created on: 9 déc. 2012
 *      Author: robin
 */

#include "Player.h"

Player::Player(websocketpp::server::connection_ptr con,const std::string& id,const std::string& name) {
	this->con = con;
	this->id = id;
	this->name=name;
}

std::string Player::getName()const{
	return this->name;
}

void Player::setname(std::string){

}

websocketpp::server::connection_ptr Player::getCon()const{
	return this->con;
}
