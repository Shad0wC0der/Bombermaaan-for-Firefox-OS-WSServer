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

Game* WSServer::addNewGame(Player* creator){
	boost::unique_lock<boost::mutex> l(lock);

	std::string id = games.size()+creator->getName();
	games.push_back(Game(id));
	return games.end();
}

void WSServer::removeGame(Game* game){
	boost::unique_lock<boost::mutex> l(lock);
}

void WSServer::on_open(websocketpp::server::handler::connection_ptr con) {
	outGameConnections.insert(std::pair<websocketpp::server::handler::connection_ptr,std::string>(con,get_con_id(con)));
    con->send("!!!");
}

void WSServer::on_message(websocketpp::server::handler::connection_ptr connection,websocketpp::server::handler::message_ptr msg) {
	RequestFactory::Request r = RequestFactory::createRequest(connection,msg->get_payload());

	//if (r!=NULL){
		coordinator.addRequest(r);
	//}
}

void WSServer::on_close(websocketpp::server::handler::connection_ptr con){
	std::map<websocketpp::server::handler::connection_ptr,std::string>::iterator it = outGameConnections.find(con);
	    if (it == outGameConnections.end()) {
	        // already disconnected
	        return;
	    }
	    outGameConnections.erase(it);
}

void process(RequestCoordinator* coordinator){
    RequestFactory::Request r;

    while (1) {
        coordinator->getRequest(r);

        r.process();
    }
}

std::string WSServer::get_con_id(websocketpp::server::handler::connection_ptr con) {
	std::stringstream endpoint;
	//endpoint << con->get_endpoint();
	endpoint << con;
	return endpoint.str();
}

int main(int argc, char* argv[]) {
	try{
		unsigned short port = 9002;
		size_t worker_threads = 3;
		//size_t pool_threads = 2;

		if (argc >= 2) {
			std::stringstream buffer(argv[1]);
			buffer >> port;
		}
		/*
		if (argc >= 3) {
			std::stringstream buffer(argv[2]);
			buffer >> pool_threads;
		}*/

		if (argc >= 3) {
			std::stringstream buffer(argv[2]);
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

		endpoint.listen(port/*,pool_threads*/);
	} catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
	return 0;
}
