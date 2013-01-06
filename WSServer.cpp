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
	delete this->requestFactory;
}

unsigned long WSServer::addNewGame(Player* creator){
	boost::mutex::scoped_lock l(lockInGamers);
	Game g = Game(creator);
	games.push_back(g);
	std::ostringstream oss;
	oss<<g.getID();
	this->notifyGameCreated(oss.str(),creator->getName());
	return games.back().getID();
}

void WSServer::removeGame(Game* game){
	boost::mutex::scoped_lock l(lockInGamers);
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
     bool found = false;

     //recherche dans les joueurs hors jeu
     boost::mutex::scoped_lock l1(lockOutGamers);
     std::list<Player*>::const_iterator iPlayer;
     iPlayer = std::find_if(outGamePlayers.cbegin(), outGamePlayers.cend(), [con](Player *p) { return p->getCon() == con; });
     if (iPlayer != outGamePlayers.cend()) {
		delete (*iPlayer);
		outGamePlayers.erase(iPlayer);
		found = true;
     }
     l1.unlock();
	 
     //recherche dans les joueurs en jeu
     if(!found) {
          boost::mutex::scoped_lock l2(lockInGamers);
          for (Game& g : games){
               if(g.tryToRemovePlayerByCon(con)) {
                    found = true;
                    break;
               }
          }
          l2.unlock();
     }

     if (!found)
          std::cerr<<"Tried to remove unexisting player."<<std::endl;
}

void process(RequestCoordinator* coordinator){
    while (1) {
		Request* r = coordinator->getRequest();
		r->process();
		delete r;
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
/*
Player* WSServer::takeOutGamePlayerByCon(connection_ptr con){
     boost::unique_lock<boost::mutex> l1(lockOutGamers);

     std::list<Player*>::const_iterator iPlayer = std::find_if(outGamePlayers.cbegin(),
																outGamePlayers.cend(),
                                                                [con] (Player *p) { return p->getCon() == con; });
     Player *p = *iPlayer;
     outGamePlayers.erase(iPlayer);

     l1.unlock();
     return p;
}*/

void WSServer::addNewPlayer(connection_ptr con,const std::string& name){
	boost::mutex::scoped_lock l(lockOutGamers);
	Player* p = new Player(con,get_con_id(con),name);
	this->notifyPlayerJoined(p->getID(),p->getName());

	outGamePlayers.push_back(p);
}
void WSServer::addNewMessage(const std::string& message,const std::string& author,const std::string& id){
	Message* m = new Message(message,id,author);
	boost::mutex::scoped_lock l2(lockOutGameMessages);
	this->chatBox.addMessage(m);
	l2.unlock();
	boost::mutex::scoped_lock l1(lockOutGamers);
	this->notifyMessageSent(m->getMessage(),m->getAuthorName(),m->getAuthorID(),m->getSubmitTime());
}



void WSServer::notifyPlayerJoined(const std::string& id,const std::string& name){
	std::string m = "{\"type\":\""+std::string(stringify(NOTIFY_PLAYER_JOINED))+"\",\"value\"={\"name\":\""+name+"\",\"id\":\""+id+"\"}}";

	for(Player* p : outGamePlayers){
		p->getCon()->send(m);
	}
}

void WSServer::notifyMessageSent(const std::string& message,const std::string& author,const std::string& id,const std::string& submitTime){
	std::string m = "{\"type\":\""+std::string(stringify(NOTIFY_MESSAGE_SENT))+"\",\"value\"={\"message\":\""+message+"\",\"author\":\""+author+"\",\"id\":\""+id+"\",\"submitTime\":\""+submitTime+"\"}}";
	
	for(Player* p : outGamePlayers){
		p->getCon()->send(m);
	}
}

void WSServer::notifyGameCreated(const std::string& id,const std::string& authorName){
	std::string m = "{\"type\":\""+std::string(stringify(NOTIFY_GAME_CREATED))+"\",\"value\"={\"id\":\""+id+"\",\"author\":\""+authorName+"\"}}";

	for(Player* p : outGamePlayers){
		p->getCon()->send(m);
	}
}

std::string WSServer::getOutGameData(){
	std::string response="{\"type\":\""+std::string(stringify(REFRESH_OUT_GAME_DATA))+"\",\"value\":{\"games\":[";
	boost::mutex::scoped_lock l1(this->lockInGamers);
	if(!(this->games.empty())){
	std::list<Game>::iterator it = this->games.begin();
	std::ostringstream oss;
	oss<<it->getID();
	std::string id=oss.str();
	response+="\""+id+"\"";
	++it;
	while(it != this->games.end()){
		std::ostringstream oss;
		oss<<it->getID();
		std::string id=oss.str();
		response+=",\""+id+"\"";
		++it;
		}
	}
	l1.unlock();
	response+="],\"players\":[";
	boost::mutex::scoped_lock l2(this->lockOutGamers);
	if(!(this->outGamePlayers.empty())){
		std::list<Player*>::iterator it = this->outGamePlayers.begin();
		std::ostringstream oss;
		oss<<"{\"name\":\""<<(*it)->getName()<<"\",\"id\":\""<<(*it)->getID()<<"\"}";
		std::string player=oss.str();
		response+=player;
		++it;
		while(it != this->outGamePlayers.end()){
			std::ostringstream oss;
			oss<<",{\"name\":\""<<(*it)->getName()<<"\",\"id\":\""<<(*it)->getID()<<"\"}";
			std::string player=oss.str();
			response+=player;
			++it;
		}
	}
	l2.unlock();
	response+="],\"messages\":[";
	boost::mutex::scoped_lock l3(this->lockOutGameMessages);
	std::list<Message*> messages=this->chatBox.getMessages();
	if(!(messages.empty())){
		std::list<Message*>::iterator it = messages.begin();
		std::ostringstream oss;
		oss<<"{\"message\":\""<<(*it)->getMessage()<<"\",\"authorName\":\""<<(*it)->getAuthorName()<<"\",\"authorID\":\""<<(*it)->getAuthorID()<<"\",\"submitTime\":\""<<(*it)->getSubmitTime()<<"\"}";
		std::string player=oss.str();
		response+=player;
		++it;
		while(it != messages.end()){
			std::ostringstream oss;
			oss<<"{\"message\":\""<<(*it)->getMessage()<<"\",\"authorName\":\""<<(*it)->getAuthorName()<<"\",\"authorID\":\""<<(*it)->getAuthorID()<<"\",\"submitTime\":\""<<(*it)->getSubmitTime()<<"\"}";
			std::string player=oss.str();
			response+=player;
			++it;
		}
	}
	l3.unlock();
	response+="]}}";
	return response;
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
		websocketpp::server::handler::ptr server_handler(&srv);
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
