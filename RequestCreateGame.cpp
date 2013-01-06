/*
 * RequestCreateGame.cpp
 *
 *  Created on: 16 déc. 2012
 *      Author: root
 */

#include "RequestCreateGame.h"
#include "WSServer.h"

void RequestCreateGame::process(void){
	/*try{
		if(!parser->getCurrentValue()->first_child || parser->getCurrentValue()->first_child->type != json_type::JSON_STRING) throw 0;
		std::string name=parser->getCurrentValue()->first_child->string_value;
	
	Player* host=this->server->takeOutGamePlayerByCon(this->con);
	unsigned long id = this->server->addNewGame(host);
	std::ostringstream oss;
	oss<<id;

	std::string response="{\"type\":\""+std::string(stringify(JOIN_GAME))+"\",\"value\"=\""+oss.str()+"\"}";
	con->send(response);
	
	}catch(int){
		con->send("wrong json format");
	}
	*/
	//this->server->addNewGame(


}