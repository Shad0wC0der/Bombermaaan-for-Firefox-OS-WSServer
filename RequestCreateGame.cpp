/*
 * RequestCreateGame.cpp
 *
 *  Created on: 16 d�c. 2012
 *      Author: root
 */

#include "RequestCreateGame.h"
#include "WSServer.h"

void RequestCreateGame::process(void){
	this->server->createGame(this->con);
}