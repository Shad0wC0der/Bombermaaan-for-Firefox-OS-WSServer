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

void RequestCoordinator::addRequest(const RequestFactory::Request& r){
	boost::unique_lock<boost::mutex> l(lock);
	requests.push(r);
	l.unlock();
	cond.notify_one();
}

void RequestCoordinator::getRequest(RequestFactory::Request& value){
	boost::unique_lock<boost::mutex> l(lock);

	while (requests.empty()) {
		cond.wait(l);
	}

	value = requests.front();
	requests.pop();
}
