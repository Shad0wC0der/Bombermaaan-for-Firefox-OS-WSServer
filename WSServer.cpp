/*
 * WSServer.cpp
 *
 *  Created on: 9 déc. 2012
 *      Author: robin
 */

#include "WSServer.h"

WSServer::WSServer() {
	this->requestFactory=new RequestFactory(this);
	this->games_ptr=new Game*[WSServer::NB_SIMULTANEOUS_GAMES];
	for(int i = 0 ; i < WSServer::NB_SIMULTANEOUS_GAMES; i++){
		games_ptr[i]=((Game*)NULL);
	}
	this->gameLockers=new boost::mutex*[WSServer::NB_SIMULTANEOUS_GAMES];
	for(int i = 0 ; i < WSServer::NB_SIMULTANEOUS_GAMES; i++){
		gameLockers[i]=((boost::mutex*)NULL);
	}
}

WSServer::~WSServer() {
	delete this->requestFactory;
	for(int i = 0 ; i < WSServer::NB_SIMULTANEOUS_GAMES; i++){
		if(games_ptr[i]!=((Game*)NULL))
			delete games_ptr[i];
	}
	delete [] this->games_ptr;
}

void WSServer::removeGame(const unsigned short& gameID){
	std::list<Game*>::iterator it= std::find_if(games.begin(),
												games.end(),
												[gameID] (Game* g) { return g->getID() == gameID; });
	games.erase(it);
	delete games_ptr[gameID];
	games_ptr[gameID]=(Game*)0;

	if(gameLockers[gameID] != ((boost::mutex*)0)){
		delete gameLockers[gameID];
		gameLockers[gameID]=((boost::mutex*)0);
	}
}

void WSServer::startGame(const unsigned short& gameID,const websocketpp::server::connection_ptr& con){
	boost::unique_lock<boost::mutex> l(lockInGamers);
	if(games_ptr[gameID] == ((Game*)NULL)){
		this->notifyError("La partie ne peut pas être lancée : la partie n'existe pas",con);
		return;
	}
	if(games_ptr[gameID]->getHost()->getCon() != con){
		this->notifyError("Seul l'hôte peut lancer une partie",con);
		return;
	}
	
	if(games_ptr[gameID]->getNbPlayers()<2){
		this->notifyError("Pas assez de joueurs",con);
		return;
	}
	
	games_ptr[gameID]->startGame();
	this->gameLockers[gameID]=new boost::mutex();
	this->runningGames.push_back(games_ptr[gameID]);

}

void WSServer::stopGame(const unsigned short& gameID){
	std::list<Game*>::iterator iGame;
	iGame = std::find_if(this->runningGames.begin(), this->runningGames.end(), [gameID](Game* game) { return game->getID() == gameID; });
	this->runningGames.erase(iGame);
	delete this->gameLockers[gameID];
	gameLockers[gameID]=((boost::mutex*)NULL);
}

void WSServer::on_open(websocketpp::server::connection_ptr con) {
	//this->addNewPlayer(con);
}


void WSServer::on_message(websocketpp::server::connection_ptr connection,websocketpp::server::handler::message_ptr msg) {
	Request* r = this->requestFactory->createRequest(connection,msg->get_payload());
	std::cout<<msg->get_payload()<<std::endl;
	if (r!=NULL){
		coordinator.addRequest(r);
	}
}

void WSServer::on_close(websocketpp::server::connection_ptr con){
	this->coordinator.addRequest(new RequestClose(con,this));
}

void process(RequestCoordinator* coordinator){
    while (1) {
		Request* r = coordinator->getRequest();
		r->process();
		delete r;
    }
}

void WSServer::tickInGame(){
	while(1){
		boost::this_thread::sleep(boost::posix_time::milliseconds(WSServer::IN_GAME_TICK));
		boost::unique_lock<boost::mutex> l1(this->lockInGamers);
		for(Game* game : this->runningGames){
			//boost::unique_lock<boost::mutex> l2(*this->gameLockers[game->getID()]);
			game->perform();
		}
		
		boost::unique_lock<boost::mutex> l2(this->stoppingGamesLocker);
		while(!this->stoppingGames.empty()){
			unsigned short i = stoppingGames.back();
			stoppingGames.pop_back();
			if(gameLockers[i]!=NULL){
				this->stopGame(i);
			}
		}
	}
}

void WSServer::redirectMoveRequest(const unsigned short& iGame,const unsigned short& iPlayer,const websocketpp::server::connection_ptr& con,const unsigned short& direction){
	if(iGame>=WSServer::NB_SIMULTANEOUS_GAMES)return;
	if(this->games_ptr[iGame] == NULL) return;
	if(this->gameLockers[iGame] == NULL) return;


	boost::unique_lock<boost::mutex> l(*this->gameLockers[iGame]);
	this->games_ptr[iGame]->move(iPlayer,con,direction);
}

void WSServer::redirectDropBombRequest(const unsigned short& iGame,const unsigned short& iPlayer,const websocketpp::server::connection_ptr& con){
	if(iGame>=WSServer::NB_SIMULTANEOUS_GAMES)return;
	if(this->games_ptr[iGame] == NULL) return;
	if(this->gameLockers[iGame] == NULL) return;

	boost::unique_lock<boost::mutex> l(*this->gameLockers[iGame]);
	this->games_ptr[iGame]->dropBomb(iPlayer,con);
}

std::string WSServer::get_con_id(const websocketpp::server::handler::connection_ptr& con) {
	std::stringstream endpoint;
	//endpoint << con->get_endpoint();
	endpoint << con;
	return endpoint.str();
}
void WSServer::createGame(const websocketpp::server::connection_ptr& hostCon){
	boost::unique_lock<boost::mutex> l1(lockOutGamers);
	boost::unique_lock<boost::mutex> l2(lockInGamers);
	if(this->games.size()>=WSServer::NB_SIMULTANEOUS_GAMES)
		this->notifyError("Impossible de créer une nouvelle partie, nombre maximal de parties simultanées atteint",hostCon);
	else {
		std::list<Player*>::iterator iPlayer = std::find_if(outGamePlayers.begin(),
																	outGamePlayers.end(),
																   [hostCon] (Player *p) { return p->getCon() == hostCon; });
		//securité
		if (iPlayer == outGamePlayers.end()){
			this->notifyError("Joueur hôte non trouvé dans la liste",hostCon);
			return ;
		}
		Player *p = *iPlayer;
		outGamePlayers.erase(iPlayer);
		l1.unlock();

		unsigned long idGame = addNewGame(p);
		std::ostringstream oss;
		oss<<idGame;

		games_ptr[idGame]->notifyEnteringRoom(p);
	}
}

void WSServer::createMessage(const std::string& message,const std::string& author,const std::string& authorID,const std::string& messageType,const std::string& target){

	Message* m;
	m = new Message(message,author,authorID);

	if(std::string(messageType).compare(stringify(TO_ALL)) == 0){
		boost::mutex::scoped_lock l1(lockOutGamers);
		boost::mutex::scoped_lock l2(lockOutGameMessages);
		this->chatBox.addMessage(m);
		this->notifyMessageSent(m->getMessage(),m->getAuthorName(),m->getAuthorID(),m->getSubmitTime());
	}else if(std::string(messageType).compare(stringify(TO_ROOM)) == 0){
	}else if(std::string(messageType).compare(stringify(TO_PLAYER)) == 0){
	
	}
}

void WSServer::createPlayer(const websocketpp::server::connection_ptr& con,const std::string& name){
	boost::mutex::scoped_lock l(lockOutGamers);
	this->addNewPlayer(con,name);
}

unsigned long WSServer::addNewGame(Player* creator){
	int i;
	for(i = 0 ; i < WSServer::NB_SIMULTANEOUS_GAMES; i++){
		if(games_ptr[i]==((Game*)NULL))
			break;
	}
	Game* g = new Game(creator,i,this);
	games.push_back(g);
	games_ptr[i]=games.back();
	std::ostringstream oss;
	oss<<g->getID();
	this->notifyGameCreated(oss.str(),creator->getName());
	return games.back()->getID();
}

void WSServer::addNewPlayer(const websocketpp::server::connection_ptr& con,const std::string& name){
	Player* p = new Player(con,get_con_id(con),name);
	this->notifyPlayerJoined(p->getID(),p->getName());

	outGamePlayers.push_back(p);
}

bool WSServer::switchPlayerToGame(const websocketpp::server::connection_ptr& con,const unsigned int& gameID){
	boost::mutex::scoped_lock l1(lockOutGamers);
	boost::mutex::scoped_lock l2(lockInGamers);

	if(gameID < WSServer::NB_SIMULTANEOUS_GAMES 
		&& games_ptr[gameID]!=(Game*)0
		&& games_ptr[gameID]->getNbPlayers() < Game::MAX_PLAYER){

		std::list<Player*>::iterator iPlayer = std::find_if(outGamePlayers.begin(),
																	outGamePlayers.end(),
																	[con] (Player *p) {return p->getCon() == con;});
		if (iPlayer == outGamePlayers.end()) {
			return false;
		}else{
			Player* p = *iPlayer;
			games_ptr[gameID]->addPlayer(p);
			outGamePlayers.erase(iPlayer);
			std::ostringstream oss;
			oss<<gameID;
			return true;
		} 

	}else return false;
}

void WSServer::selectMapForGame(const unsigned short& mapID,const unsigned short& gameID,const websocketpp::server::connection_ptr& con){
	boost::mutex::scoped_lock l(lockInGamers);
	if(gameID>=WSServer::NB_SIMULTANEOUS_GAMES
	||games_ptr[gameID] == ((Game*)NULL)
	||games_ptr[gameID]->getHost()->getCon()!=con
	||gameLockers[gameID] != ((boost::mutex*)NULL)
	||Map::NB_SERVER_MAP <= mapID) 
		return;

	games_ptr[gameID]->getMap()->setSMap(Map::maps[mapID]);

	for(int i = 0 ; i < games_ptr[gameID]->getNbPlayers(); i++){
		this->sendGameMapData(games_ptr[gameID]->getPlayer(i)->getCon(),gameID);
	}
}

void WSServer::chooseColor(const unsigned short& gameID, const unsigned short& color, const websocketpp::server::connection_ptr& con){
	boost::mutex::scoped_lock l(lockInGamers);
	if(gameID>=WSServer::NB_SIMULTANEOUS_GAMES
	||games_ptr[gameID] == ((Game*)NULL)
	||!games_ptr[gameID]->isInGame(con)
	||gameLockers[gameID] != ((boost::mutex*)NULL))
		return;

	games_ptr[gameID]->changePlayerColor(con,color);

	games_ptr[gameID]->notifyColorChanged();
}

void WSServer::addGameToStoppingGames(const unsigned short& id){
	boost::mutex::scoped_lock l(this->stoppingGamesLocker);
	if(id < NB_SIMULTANEOUS_GAMES && this->gameLockers[id] != ((boost::mutex*)NULL)){
		this->stoppingGames.push_back(id);
	}
}

void WSServer::notifyPlayerJoined(const std::string& id,const std::string& name){
	std::string m = "{\"type\":\""+std::string(stringify(NOTIFY_PLAYER_JOINED))+"\",\"value\":{\"name\":\""+name+"\",\"id\":\""+id+"\"}}";

	for(Player* p : outGamePlayers){
		p->getCon()->send(m);
	}
}

void WSServer::notifyMessageSent(const std::string& message,const std::string& author,const std::string& id,const std::string& submitTime){
	std::string m = "{\"type\":\""+std::string(stringify(NOTIFY_MESSAGE_SENT))+"\",\"value\":{\"message\":\""+message+"\",\"author\":\""+author+"\",\"id\":\""+id+"\",\"submitTime\":\""+submitTime+"\"}}";
	
	for(Player* p : outGamePlayers){
		p->getCon()->send(m);
	}
}

void WSServer::notifyGameCreated(const std::string& id,const std::string& authorName){
	std::string m = "{\"type\":\""+std::string(stringify(NOTIFY_GAME_CREATED))+"\",\"value\":{\"id\":\""+id+"\",\"author\":\""+authorName+"\"}}";

	for(Player* p : outGamePlayers){
		p->getCon()->send(m);
	}
}

void WSServer::notifyPlayerExited(const std::string& id){
	std::string m = "{\"type\":\""+std::string(stringify(NOTIFY_PLAYER_EXITED))+"\",\"value\":{\"id\":\""+id+"\"}}";

	for(Player* p : outGamePlayers){
		p->getCon()->send(m);
	}
}

void WSServer::notifyError(const std::string& error,const websocketpp::server::connection_ptr& receiver){
	std::string m = "{\"type\":\""+std::string(stringify(NOTIFY_ERROR))+"\",\"value\":{\"error\":\""+error+"\"}}";
	receiver->send(m);
}

void WSServer::notifyGameRemoved(const std::string& gameID){
	std::string m = "{\"type\":\""+std::string(stringify(NOTIFY_GAME_REMOVED))+"\",\"value\":{\"id\":\""+gameID+"\"}}";
	for(Player* p : outGamePlayers){
		p->getCon()->send(m);
	}
}

void WSServer::sendGameMapList(const websocketpp::server::connection_ptr& con){
	if(Map::NB_SERVER_MAP>0){
		std::string m = "{\"type\":\""+std::string(stringify(GAME_MAP_LIST))+"\",\"value\":{\"maps\":[\""+Map::maps[0]+"\"";
		for(int i = 1 ; i < Map::NB_SERVER_MAP ; i++){
			m+=",\""+Map::maps[i]+"\"";
		}
		m+="]}}";
		con->send(m);
	}else{
		this->notifyError("Pas de map disponible sur le serveur",con);
	}
}

void WSServer::sendGameMapData(const websocketpp::server::connection_ptr& con,const unsigned short& gameID){
	std::ostringstream oss;
	oss<<gameID;

	std::string m = "{\"type\":\""+std::string(stringify(GAME_MAP_DATA))+"\",\"value\":{\"id\":\""+oss.str()+"\",\"map\":\""+games_ptr[gameID]->getMap()->getCurrentSMap()+"\"}";
	con->send(m);
}

void WSServer::closeConnection(const websocketpp::server::connection_ptr& con){
	bool found = false;

     //recherche dans les joueurs hors jeu
     boost::mutex::scoped_lock l1(lockOutGamers);

     std::list<Player*>::iterator iPlayer;
     iPlayer = std::find_if(outGamePlayers.begin(), outGamePlayers.end(), [con](Player *p) { return p->getCon() == con; });
     if (iPlayer != outGamePlayers.end()) {
		 std::string id = (*iPlayer)->getID();
		 delete (*iPlayer);
		 outGamePlayers.erase(iPlayer);
		 this->notifyPlayerExited(id);
		 found = true;
     }
	 
     //recherche dans les joueurs en jeu
     if(!found) {
        boost::mutex::scoped_lock l2(lockInGamers);
		for (Game* g : games){
			if(gameLockers[g->getID()] == ((boost::mutex*)NULL)){
				if(g->tryToRemovePlayerByCon(con)) {
					found = true;
					std::ostringstream oss;
					oss<<g->getID();
					std::string playerID = oss.str();
					if(g->getNbPlayers() == 0){
						
						this->notifyGameRemoved(playerID);
						this->removeGame(g->getID());
					break;
					}else{
						g->notifyExitingRoom(playerID);
					}
				}
			}else{
				boost::mutex::scoped_lock l3(*gameLockers[g->getID()]);
				if(g->tryToRemovePlayerByCon(con)) {
					found = true;
					std::ostringstream oss;
					oss<<g->getID();
					std::string playerID = oss.str();
					if(g->getNbPlayers() == 0){
						this->notifyGameRemoved(playerID);
						this->removeGame(g->getID());
					break;
					}else{
						g->notifyExitingRoom(playerID);
					}
				}
			}
		}
          l2.unlock();
     }

     if (!found)
          std::cerr<<"Tried to remove unexisting player."<<std::endl;
}

std::string WSServer::getOutGameData(){
	
	std::string response="{\"type\":\""+std::string(stringify(REFRESH_OUT_GAME_DATA))+"\",\"value\":{\"games\":[";
	boost::mutex::scoped_lock l1(this->lockInGamers);
	if(!(this->games.empty())){
	std::list<Game*>::iterator it = this->games.begin();
	std::ostringstream oss;
	oss<<(*it)->getID();
	std::string id=oss.str();
	response+="\""+id+"\"";
	++it;
	while(it != this->games.end()){
		std::ostringstream oss;
		oss<<(*it)->getID();
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
			oss<<",{\"message\":\""<<(*it)->getMessage()<<"\",\"authorName\":\""<<(*it)->getAuthorName()<<"\",\"authorID\":\""<<(*it)->getAuthorID()<<"\",\"submitTime\":\""<<(*it)->getSubmitTime()<<"\"}";
			std::string player=oss.str();
			response+=player;
			++it;
		}
	}
	l3.unlock();
	response+="]}}";
	return response;
}

void tickInGame(WSServer* srv){
	srv->tickInGame();
}

int main(int argc, char* argv[]) {
	try{
		unsigned short port = 27016;
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
					new boost::thread(boost::bind(tickInGame,&srv))
				)
			);
		}

		endpoint.listen(port/*,pool_threads*/);
	} catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
	return 0;
}
