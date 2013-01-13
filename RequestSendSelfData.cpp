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
		this->server->createPlayer(this->con,name);
	}catch(int){
		con->send("wrong json format");
	}
}