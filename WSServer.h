/*
 * WSServer.h
 *
 *  Created on: 9 déc. 2012
 *      Author: robin
 */
#ifndef WSSERVER_H_
#define WSSERVER_H_
#include <websocketpp/websocketpp.hpp>
#include "RequestCoordinator.h"

void process(RequestCoordinator*);
class WSServer : public websocketpp::server::handler {
public:
	std::map<connection_ptr,std::string> connections;
	void on_open(connection_ptr con);
	void on_message(connection_ptr connection,message_ptr msg);
	void on_close(connection_ptr con);
	std::string get_con_id(connection_ptr con);
	WSServer();
	virtual ~WSServer();
	RequestCoordinator* getCoordinator(){return &coordinator;}
private:
	RequestCoordinator coordinator;
};

#endif /* WSSERVER_H_ */
