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
class Request{

protected:
	websocketpp::server::connection_ptr con;

public:
	Request(websocketpp::server::handler::connection_ptr con){this->con = con;}
	virtual ~Request(){}
	virtual void process(void);
};

#endif /* REQUEST_H_ */
