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

RequestFactory::Request* RequestFactory::createRequest(websocketpp::server::handler::connection_ptr* con,std::string source){
	//parsing json

	char *errorPos = 0;
	char *errorDesc = 0;
	int errorLine = 0;
	block_allocator allocator(1 << 10); // 1 KB per block

	json_value *root = json_parse( (char *)source.c_str(), &errorPos, &errorDesc, &errorLine, &allocator);
	json_value *value = root->first_child;

	if(!strcmp(value->string_value,"message"))
		std::cout<<"mess : "<<value->string_value;




	return (RequestFactory::Request*)NULL;
}
