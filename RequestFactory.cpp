/*
 * RequestFactory.cpp
 *
 *  Created on: 16 déc. 2012
 *      Author: root
 */

#include "RequestFactory.h"

RequestFactory::RequestFactory() {
	// TODO Auto-generated constructor stub

}

RequestFactory::~RequestFactory() {
	// TODO Auto-generated destructor stub
}

Request* RequestFactory::createRequest(websocketpp::server::handler::connection_ptr con,std::string source){
	//parsing json

	JsonParser* parser = new JsonParser(source);

	if(parser->getCurrentValue()){
		if(std::string((parser->getCurrentValue()->string_value)).compare(stringify(MESSAGE)) == 0){
			if(parser->nextValue()){
				return new RequestMessage(parser,con);
			}
		}
	}
	
	return new Request(con);
}
