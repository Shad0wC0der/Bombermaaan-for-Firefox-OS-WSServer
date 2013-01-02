/*
 * RequestMessage.cpp
 *
 *  Created on: 16 déc. 2012
 *      Author: root
 */

#include "RequestMessage.h"
#include "WSServer.h"
void RequestMessage::process(void){
	try{	
		
		if(!parser->getCurrentValue()->first_child || parser->getCurrentValue()->first_child->type != json_type::JSON_STRING) throw 0;
		std::string message = (this->parser->getCurrentValue()->first_child->string_value);
		std::string author = this->parser->getCurrentValue()->next_sibling->string_value;
		this->server->notifyMessageSent(message,author,this->server->get_con_id(con));
	}catch(int){
		con->send("wrong json format");
	}

}