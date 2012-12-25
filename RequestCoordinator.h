/*
 * RequestCoordinator.h
 *
 *  Created on: 13 déc. 2012
 *      Author: root
 */

#ifndef REQUESTCOORDINATOR_H_
#define REQUESTCOORDINATOR_H_
#include "RequestFactory.h"

class RequestCoordinator {
	public:
		RequestCoordinator();
		virtual ~RequestCoordinator();

	    void addRequest(Request* r) ;
	    Request* getRequest() ;
	private:
	    std::queue<Request*>		requests;
	    boost::mutex                lock;
	    boost::condition_variable   cond;
};

#endif /* REQUESTCOORDINATOR_H_ */
