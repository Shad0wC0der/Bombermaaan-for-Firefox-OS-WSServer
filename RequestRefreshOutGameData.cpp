/*
 * RequestRefreshOutGameData.cpp
 *
 *  Created on: 16 déc. 2012
 *      Author: root
 */

#include "RequestRefreshOutGameData.h"
#include "WSServer.h"

void RequestRefreshOutGameData::process(void){
	
	con->send(this->server->getOutGameData());
}