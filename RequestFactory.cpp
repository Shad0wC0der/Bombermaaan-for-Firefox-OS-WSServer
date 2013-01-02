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

Request* RequestFactory::createRequest(websocketpp::server::handler::connection_ptr con,std::string source){
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
		else if(std::string((parser->getCurrentValue()->string_value)).compare(stringify(REFRESH_OUT_GAME_DATA)) == 0){
			return new RequestRefreshOutGameData(con,this->server);
		}
	}
	
	return new Request(con);
}
