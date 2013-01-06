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
		json_value* jv = parser->getCurrentValue()->first_child;
		if(!jv || jv->type != json_type::JSON_STRING) throw 0;
		std::string message = jv->string_value;
		jv=jv->next_sibling;
		if(!jv || jv->type != json_type::JSON_STRING)throw 0;
		std::string author = jv->string_value;
		this->server->addNewMessage(message,author,this->server->get_con_id(con));

	}catch(int){
		con->send("wrong json format");
	}

}