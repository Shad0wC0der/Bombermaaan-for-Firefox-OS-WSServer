/*
 * RequestFactory.h
 *
 *  Created on: 16 déc. 2012
 *      Author: root
 */

#ifndef REQUESTFACTORY_H_
#define REQUESTFACTORY_H_
#include <websocketpp/websocketpp.hpp>
#include "vjson/json.h"

class RequestFactory {
public:
	struct Request{
		void process(){con->send("!!!");}
		websocketpp::server::handler::connection_ptr	 con;
		json_value*                   			 		 value;
	};

	RequestFactory();
	virtual ~RequestFactory();
	static Request createRequest(websocketpp::server::handler::connection_ptr,std::string);
};

#endif /* REQUESTFACTORY_H_ */
