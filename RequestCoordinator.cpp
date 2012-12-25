/*
 * RequestCoordinator.cpp
 *
 *  Created on: 13 déc. 2012
 *      Author: root
 */

#include "RequestCoordinator.h"

RequestCoordinator::RequestCoordinator() {
	// TODO Auto-generated constructor stub
}

RequestCoordinator::~RequestCoordinator() {
	// TODO Auto-generated destructor stub
}

void RequestCoordinator::addRequest(Request* r){
	boost::unique_lock<boost::mutex> l(lock);
	requests.push(r);
	l.unlock();
	cond.notify_one();
}

Request* RequestCoordinator::getRequest(){
	boost::unique_lock<boost::mutex> l(lock);

	while (requests.empty()) {
		cond.wait(l);
	}

	Request* r = requests.front();
	requests.pop();
	return r;
}
