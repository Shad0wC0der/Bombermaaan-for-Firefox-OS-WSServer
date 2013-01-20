/*
 * Request.h
 *
 *  Created on: 9 déc. 2012
 *      Author: robin
 */

#ifndef REQUEST_H_
#define REQUEST_H_
#include <websocketpp/websocketpp.hpp>
#include "vjson/json.h"
#include "JsonParser.h"
class WSServer;
class Request{

protected:
	websocketpp::server::connection_ptr con;

public:
	Request(websocketpp::server::handler::connection_ptr con){this->con = con;}
	virtual ~Request(){}
	virtual void process(void);
};

class RequestClose : public Request{
public:
	RequestClose(const websocketpp::server::handler::connection_ptr& con,WSServer* server) : Request(con),server(server){}
	void process(void);
	virtual ~RequestClose(){}
private:
	WSServer* server;
};

class RequestCreateGame : public Request{
public:
	RequestCreateGame(const websocketpp::server::handler::connection_ptr& con,WSServer* server) : Request(con),server(server){}
	void process(void);
	virtual ~RequestCreateGame(){}
private:
	WSServer* server;
};

class RequestJoinGame : public Request{
public:
	RequestJoinGame(JsonParser* parser,const websocketpp::server::handler::connection_ptr& con,WSServer* server) : Request(con),server(server){}
	void process(void);
	virtual ~RequestJoinGame(){delete parser;}
private:
	WSServer* server;
	JsonParser* parser;
};

class RequestMessage : public Request{
public:
	enum MESSAGE_TYPE {TO_ALL,
					   TO_ROOM,
					   TO_PLAYER};

	RequestMessage(JsonParser* parser,const websocketpp::server::handler::connection_ptr& con,WSServer* server) : Request(con),parser(parser),server(server){}
	void process(void);
	virtual ~RequestMessage(){delete parser;}
private:
	 JsonParser* parser;
	 WSServer* server;
};

class RequestRefreshOutGameData : public Request{
public:
	RequestRefreshOutGameData(const websocketpp::server::handler::connection_ptr& con,WSServer* server) : Request(con),server(server){}
	void process(void);
	virtual ~RequestRefreshOutGameData(){}
private:
	WSServer* server;
};

class RequestSendSelfData : public Request{
public:
	RequestSendSelfData(JsonParser* parser,const websocketpp::server::handler::connection_ptr& con,WSServer* server) : Request(con),parser(parser),server(server){}
	void process(void);
	virtual ~RequestSendSelfData(){delete parser;}
private:
	 JsonParser* parser;
	 WSServer* server;
};
#endif /* REQUEST_H_ */
