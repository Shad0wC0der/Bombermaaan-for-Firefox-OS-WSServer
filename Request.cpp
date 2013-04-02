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

void RequestJoinGame::process(void){
	try{
		json_value* jv = parser->getCurrentValue()->first_child;
		if(!jv || jv->type != json_type::JSON_STRING) throw 0;
		std::string gameID = jv->string_value;

		int igameid=atoi(gameID.c_str());
		if(igameid==0 && gameID.compare("0")!=0)return;//security

		this->server->switchPlayerToGame(this->con,igameid);
	}catch(int){
		con->send("wrong json format");
	}
}

void RequestRefreshOutGameData::process(void){
	con->send(this->server->getOutGameData(this->con));
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

void RequestLaunchGame::process(void){
	try{
		if(!parser->getCurrentValue()->first_child || parser->getCurrentValue()->first_child->type != json_type::JSON_STRING) throw 0;
		std::string id = parser->getCurrentValue()->first_child->string_value;
		int iid=atoi(id.c_str());
		if(iid==0 && id.compare("0")!=0)return;//security
		this->server->startGame(iid,this->con);
	}catch(int){
		con->send("wrong json format");
	}
}

void RequestSelectMap::process(void){
	try{
		json_value* jv = parser->getCurrentValue()->first_child;
		if(!jv || jv->type != json_type::JSON_STRING) throw 0;
		std::string mapID = jv->string_value;
		jv=jv->next_sibling;
		if(!jv || jv->type != json_type::JSON_STRING)throw 0;
		std::string gameID = jv->string_value;


		int imapid=atoi(mapID.c_str());
		if(imapid==0 && mapID.compare("0")!=0)return;//security

		int igameid=atoi(gameID.c_str());
		if(igameid==0 && gameID.compare("0")!=0)return;//security

		this->server->selectMapForGame(imapid,igameid,this->con);
	}catch(int){
		con->send("wrong json format");
	}
}

void RequestChooseColor::process(void){
	try{
		json_value* jv = parser->getCurrentValue()->first_child;
		if(!jv || jv->type != json_type::JSON_STRING) throw 0;
		std::string gameID = jv->string_value;
		jv=jv->next_sibling;
		if(!jv || jv->type != json_type::JSON_STRING)throw 0;
		std::string color = jv->string_value;


		int igameid=atoi(gameID.c_str());
		if(igameid==0 && gameID.compare("0")!=0)return;//security

		int icolor=atoi(color.c_str());
		if(icolor==0 && color.compare("0")!=0)return;//security

		this->server->chooseColor(igameid,icolor,this->con);
	}catch(int){
		con->send("wrong json format");
	}
}

void RequestMove::process(void){
	try{
		json_value* jv = parser->getCurrentValue()->first_child;
		if(!jv || jv->type != json_type::JSON_STRING) throw 0;
		std::string gameID = jv->string_value;
		jv=jv->next_sibling;
		if(!jv || jv->type != json_type::JSON_STRING)throw 0;
		std::string playerSlot = jv->string_value;
		jv=jv->next_sibling;
		if(!jv || jv->type != json_type::JSON_STRING)throw 0;
		std::string direction = jv->string_value;


		int igameid=atoi(gameID.c_str());
		if(igameid==0 && gameID.compare("0")!=0)return;//security

		int iPlayerSlot=atoi(playerSlot.c_str());
		if(iPlayerSlot==0 && playerSlot.compare("0")!=0)return;//security

		int iDirection=atoi(direction.c_str());
		if(iDirection==0 && direction.compare("0")!=0)return;//security

		this->server->redirectMoveRequest(igameid,iPlayerSlot,this->con,iDirection);
	}catch(int){
		con->send("wrong json format");
	}
}

void RequestDropBomb::process(void){
	try{
		json_value* jv = parser->getCurrentValue()->first_child;
		if(!jv || jv->type != json_type::JSON_STRING) throw 0;
		std::string gameID = jv->string_value;
		jv=jv->next_sibling;
		if(!jv || jv->type != json_type::JSON_STRING)throw 0;
		std::string playerSlot = jv->string_value;
		jv=jv->next_sibling;


		int igameid=atoi(gameID.c_str());
		if(igameid==0 && gameID.compare("0")!=0)return;//security

		int iPlayerSlot=atoi(playerSlot.c_str());
		if(iPlayerSlot==0 && playerSlot.compare("0")!=0)return;//security

		this->server->redirectDropBombRequest(igameid,iPlayerSlot,this->con);
	}catch(int){
		con->send("wrong json format");
	}
}
