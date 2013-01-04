/*
 * WSServer.cpp
 *
 *  Created on: 9 déc. 2012
 *      Author: robin
 */

#include "WSServer.h"

WSServer::WSServer() {
	this->requestFactory=new RequestFactory(this);
	Game::ids=0;
}

WSServer::~WSServer() {
	this->requestFactory->~RequestFactory();
}

unsigned long WSServer::addNewGame(Player* creator){
	boost::unique_lock<boost::mutex> l(lockInGamers);
	games.push_back(Game(creator));
	return games.back().getID();
}

void WSServer::removeGame(Game* game){
	boost::unique_lock<boost::mutex> l(lockInGamers);
}

void WSServer::on_open(websocketpp::server::handler::connection_ptr con) {
	//this->addNewPlayer(con);
}

void WSServer::on_message(websocketpp::server::handler::connection_ptr connection,websocketpp::server::handler::message_ptr msg) {
	Request* r = this->requestFactory->createRequest(connection,msg->get_payload());
	std::cout<<msg->get_payload();
	//if (r!=NULL){
		coordinator.addRequest(r);
	//}
}

void WSServer::on_close(websocketpp::server::handler::connection_ptr con){

	//recherche dans les joueurs hors jeu
	boost::unique_lock<boost::mutex> l1(lockOutGamers);
	for (std::list<Player*>::iterator it = outGamePlayers.begin(); it != outGamePlayers.end(); ){
		if((*it)->getCon() == con){
			outGamePlayers.erase(it);
			(*it)->~Player();
			return;
		}
		else 
			++it;
	}
	l1.unlock();

	//recherche dans les joueurs en jeu
	boost::unique_lock<boost::mutex> l2(lockInGamers);
	for (std::list<Game>::iterator it = games.begin(); it != games.end();){
		if(it->tryToRemovePlayerByCon(con))
			return;
		else ++it;
	}
	l2.unlock();

	std::cerr<<"Tried to remove unexisting player."<<std::endl;
}

void process(RequestCoordinator* coordinator){
    while (1) {
		Request* r = coordinator->getRequest();
		r->process();
		r->~Request();
    }
}

void tickInGame(){
	while(1){
		boost::this_thread::sleep(boost::posix_time::seconds(WSServer::IN_GAME_TICK) );
	}
}

std::string WSServer::get_con_id(websocketpp::server::handler::connection_ptr con) {
	std::stringstream endpoint;
	//endpoint << con->get_endpoint();
	endpoint << con;
	return endpoint.str();
}

Player* WSServer::takeOutGamePlayerByCon(connection_ptr con){
	boost::unique_lock<boost::mutex> l1(lockOutGamers);
	for (std::list<Player*>::iterator it = outGamePlayers.begin(); it != outGamePlayers.end(); ){
		if((*it)->getCon() == con){
			outGamePlayers.erase(it);
			(*it)->~Player();
			return (*it);
		}
		else 
			++it;
	}
	l1.unlock();
	return ((Player*)0);
}

void WSServer::addNewPlayer(connection_ptr con,std::string name){
	boost::unique_lock<boost::mutex> l1(lockOutGamers);
	Player* p = new Player(con,get_con_id(con),name);
	outGamePlayers.push_back(p);
}

void WSServer::notifyPlayerJoined(std::string id,std::string name){
	std::string m = "{\"type\":\""+std::string(stringify(NOTIFY_PLAYER_JOINED))+"\",\"value\"={\"name\":\""+name+"\",\"id\":\""+id+"\"}}";
	for(std::list<Player*>::iterator it = outGamePlayers.begin();it!=outGamePlayers.end();){
		(*it)->getCon()->send(m);
	}
}

void WSServer::notifyMessageSent(std::string message,std::string author,std::string id){
	std::string m = "{\"type\":\""+std::string(stringify(NOTIFY_MESSAGE_SENT))+"\",\"value\"={\"message\":\""+message+"\",\"author\":\""+author+"\",\"id\":\""+id+"\"}}";
	for(std::list<Player*>::iterator it = outGamePlayers.begin();it!=outGamePlayers.end();){
		(*it)->getCon()->send(m);
	}
	this->chatBox.addMessage(new Message(message,id,author));
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
			/* 1 thread pour le rafraichissement en jeu */
			threads.push_back(
				boost::shared_ptr<boost::thread>(
					new boost::thread(boost::bind(&tickInGame))
				)
			);
		}

		endpoint.listen(port/*,pool_threads*/);
	} catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
	return 0;
}
