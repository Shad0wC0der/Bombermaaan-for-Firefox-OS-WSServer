/*
 * RequestSendSelfData.cpp
 *
 *  Created on: 16 déc. 2012
 *      Author: root
 */

#include "RequestSendSelfData.h"
#include "WSServer.h"

void RequestSendSelfData::process(void){
	try{
		if(!parser->getCurrentValue()->first_child || parser->getCurrentValue()->first_child->type != json_type::JSON_STRING) throw 0;
		std::string name = parser->getCurrentValue()->first_child->string_value;
		this->server->notifyPlayerJoined(this->server->get_con_id(con),name);
		this->server->addNewPlayer(this->con,name);
	}catch(int){
		con->send("wrong json format");
	}
}