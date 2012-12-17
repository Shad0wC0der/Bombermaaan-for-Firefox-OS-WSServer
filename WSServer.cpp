/*
 * WSServer.cpp
 *
 *  Created on: 9 déc. 2012
 *      Author: robin
 */

#include "WSServer.h"

WSServer::WSServer() {
}

WSServer::~WSServer() {
	// TODO Auto-generated destructor stub
}

void WSServer::on_open(connection_ptr con) {
    connections.insert(std::pair<connection_ptr,std::string>(con,get_con_id(con)));
}

void WSServer::on_message(connection_ptr connection,message_ptr msg) {
	RequestFactory::Request* r = RequestFactory::createRequest(&connection,msg->get_payload());

	if (r!=(RequestFactory::Request*)0){
		coordinator.addRequest(r);
	}
}

void WSServer::on_close(connection_ptr con){
	std::map<connection_ptr,std::string>::iterator it = connections.find(con);
	    if (it == connections.end()) {
	        // already disconnected
	        return;
	    }
	    connections.erase(it);
}

void process(RequestCoordinator* coordinator){
    RequestFactory::Request r;

    while (1) {
        coordinator->getRequest(r);

        r.process();
    }
}

std::string WSServer::get_con_id(connection_ptr con) {
	std::stringstream endpoint;
	//endpoint << con->get_endpoint();
	endpoint << con;
	return endpoint.str();
}

int main(int argc, char* argv[]) {
    unsigned short port = 9002;
    size_t worker_threads = 3;
    size_t pool_threads = 2;

    if (argc >= 2) {
        std::stringstream buffer(argv[1]);
        buffer >> port;
    }

    if (argc >= 3) {
        std::stringstream buffer(argv[2]);
        buffer >> pool_threads;
    }

    if (argc >= 4) {
        std::stringstream buffer(argv[3]);
        buffer >> worker_threads;
    }

    WSServer srv;
	websocketpp::server::handler::ptr  server_handler = websocketpp::server::handler::ptr(&srv);
	websocketpp::server endpoint(server_handler);

	std::list<boost::shared_ptr<boost::thread> > threads;

	if (worker_threads > 0) {
		for (size_t i = 0; i < worker_threads; i++) {
			threads.push_back(
				boost::shared_ptr<boost::thread>(
					new boost::thread(boost::bind(&process,srv.getCoordinator()))
				)
			);
		}
	}

	endpoint.listen(port,pool_threads);
	return 0;
}
