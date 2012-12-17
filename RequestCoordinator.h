/*
 * RequestCoordinator.h
 *
 *  Created on: 13 déc. 2012
 *      Author: root
 */

#ifndef REQUESTCOORDINATOR_H_
#define REQUESTCOORDINATOR_H_
#include <websocketpp/websocketpp.hpp>
#include "RequestFactory.h"

class RequestCoordinator {
	public:
		RequestCoordinator();
		virtual ~RequestCoordinator();

	    void addRequest(const RequestFactory::Request* r) ;
	    void getRequest(RequestFactory::Request& value) ;
	private:
	    std::queue<RequestFactory::Request>         requests;
	    boost::mutex                				lock;
	    boost::condition_variable   				cond;
};

#endif /* REQUESTCOORDINATOR_H_ */
