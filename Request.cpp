#include "Request.h"
#include "WSServer.h"

void Request::process(void){
	(con)->send("nothing to do");
}

void RequestClose::process(void){
	this->server->closeConnection(this->con);
}

void RequestCreateGame::process(void){
	this->server->createGame(this->con);
}

void RequestRefreshOutGameData::process(void){
	con->send(this->server->getOutGameData());
}

void RequestMessage::process(void){
	try{	
		json_value* jv = parser->getCurrentValue()->first_child;
		if(!jv || jv->type != json_type::JSON_STRING) throw 0;
		std::string message = jv->string_value;
		jv=jv->next_sibling;
		if(!jv || jv->type != json_type::JSON_STRING)throw 0;
		std::string author = jv->string_value;
		jv=jv->next_sibling;
		if(!jv || jv->type != json_type::JSON_STRING)throw 0;
		std::string messageType = jv->string_value;
		jv=jv->next_sibling;
		if(messageType != stringify(TO_ALL)){
			if(!jv || jv->type != json_type::JSON_STRING)throw 0;
			std::string target = jv->string_value;
			this->server->createMessage(message,author,this->server->get_con_id(con),messageType,target);
		}else this->server->createMessage(message,author,this->server->get_con_id(con),messageType,"NULL");

	}catch(int){
		con->send("wrong json format");
	}
}

void RequestSendSelfData::process(void){
	try{
		if(!parser->getCurrentValue()->first_child || parser->getCurrentValue()->first_child->type != json_type::JSON_STRING) throw 0;
		std::string name = parser->getCurrentValue()->first_child->string_value;
		this->server->createPlayer(this->con,name);
	}catch(int){
		con->send("wrong json format");
	}
}