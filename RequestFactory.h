/*
 * RequestFactory.h
 *
 *  Created on: 16 déc. 2012
 *      Author: root
 */

#ifndef REQUESTFACTORY_H_
#define REQUESTFACTORY_H_
#include "vjson/json.h"
#include "RequestMessage.h"
#include "Request.h"
#include "JsonParser.h"

class RequestFactory {
public:
	#define stringify( name ) # name
	enum REQUEST_TYPE {MESSAGE};

	RequestFactory();
	virtual ~RequestFactory();
	static Request* createRequest(websocketpp::server::handler::connection_ptr,std::string);
};

#endif /* REQUESTFACTORY_H_ */
