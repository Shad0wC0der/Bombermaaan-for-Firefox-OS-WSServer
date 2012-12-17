/*
 * RequestMessage.h
 *
 *  Created on: 16 déc. 2012
 *      Author: root
 */

#ifndef REQUESTMESSAGE_H_
#define REQUESTMESSAGE_H_
#include "RequestFactory.h"
#include <websocketpp/websocketpp.hpp>

class RequestMessage : RequestFactory::Request{
public:
	RequestMessage(json_value*,websocketpp::server::handler::connection_ptr*);
	virtual ~RequestMessage();
};

#endif /* REQUESTMESSAGE_H_ */
