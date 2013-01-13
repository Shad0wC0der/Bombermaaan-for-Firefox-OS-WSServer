/*
 * RequestSendSelfData.h
 *
 *  Created on: 16 déc. 2012
 *      Author: root
 */

#ifndef REQUESTSENDSELFDATA_H_
#define REQUESTSENDSELFDATA_H_
#include "RequestFactory.h"
#include "Request.h"
#include "JsonParser.h"

class WSServer;
class RequestSendSelfData : public Request{
public:
	RequestSendSelfData(JsonParser* parser,const websocketpp::server::handler::connection_ptr& con,WSServer* server) : Request(con),parser(parser),server(server){}
	void process(void);
	virtual ~RequestSendSelfData(){delete parser;}
private:
	 JsonParser* parser;
	 WSServer* server;
};

#endif /* REQUESTSENDSELFDATA_H_ */
