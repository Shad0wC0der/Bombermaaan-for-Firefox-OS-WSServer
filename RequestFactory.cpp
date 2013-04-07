/*
 * RequestFactory.cpp
 *
 *  Created on: 16 déc. 2012
 *      Author: root
 */

#include "RequestFactory.h"
#include "WSServer.h"

RequestFactory::RequestFactory(WSServer* server){
	this->server=server;
}

Request* RequestFactory::createRequest(const websocketpp::server::handler::connection_ptr& con,const std::string& source){
	//parsing json

	JsonParser* parser = new JsonParser(source);

	if(parser->getCurrentValue()){
		if(std::string((parser->getCurrentValue()->string_value)).compare(stringify(MESSAGE)) == 0){
			if(parser->nextValue()){
				return new RequestMessage(parser,con,this->server);
			}
		}
		else if(std::string((parser->getCurrentValue()->string_value)).compare(stringify(SEND_SELF_DATA)) == 0){
			if(parser->nextValue()){
				return new RequestSendSelfData(parser,con,this->server);
			}
		}
		else if(std::string((parser->getCurrentValue()->string_value)).compare(stringify(LAUNCH_GAME)) == 0){
			if(parser->nextValue()){
				return new RequestLaunchGame(parser,con,this->server);
			}
		}
		else if(std::string((parser->getCurrentValue()->string_value)).compare(stringify(REFRESH_OUT_GAME_DATA)) == 0){
			delete parser;
			return new RequestRefreshOutGameData(con,this->server);
		}
		else if(std::string((parser->getCurrentValue()->string_value)).compare(stringify(CREATE_GAME)) == 0){
			delete parser;
			return new RequestCreateGame(con,this->server);
		}
		else if(std::string((parser->getCurrentValue()->string_value)).compare(stringify(JOIN_GAME)) == 0){
			if(parser->nextValue()){
				return new RequestJoinGame(parser,con,this->server);
			}
		}
		else if(std::string((parser->getCurrentValue()->string_value)).compare(stringify(SELECT_SERVER_MAP)) == 0){
			return new RequestSelectMap(parser,con,this->server);
		}
		else if(std::string((parser->getCurrentValue()->string_value)).compare(stringify(CHOOSE_COLOR)) == 0){
			return new RequestChooseColor(parser,con,this->server);
		}
		else if(std::string((parser->getCurrentValue()->string_value)).compare(stringify(MOVE)) == 0){
			if(parser->nextValue()){
				return new RequestMove(parser,con,this->server);
			}
		}
		else if(std::string((parser->getCurrentValue()->string_value)).compare(stringify(DROP_BOMB)) == 0){
			if(parser->nextValue()){
				return new RequestDropBomb(parser,con,this->server);
			}
		}
	}
	
	return new Request(con);
}
