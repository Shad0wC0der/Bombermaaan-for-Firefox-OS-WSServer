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
	RequestSendSelfData(JsonParser* parser,websocketpp::server::handler::connection_ptr con,WSServer* server) : Request(con){this->parser=parser;this->server=server;}
	void process(void);
	virtual ~RequestSendSelfData(){parser->~JsonParser();}
private:
	 JsonParser* parser;
	 WSServer* server;
};

#endif /* REQUESTSENDSELFDATA_H_ */
