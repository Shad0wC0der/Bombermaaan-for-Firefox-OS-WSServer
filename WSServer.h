/*
 * WSServer.h
 *
 *  Created on: 9 déc. 2012
 *      Author: robin
 */

#ifndef WSSERVER_H_
#define WSSERVER_H_
#include <sstream>
#include <algorithm>
#include <list>
#include "RequestCoordinator.h"
#include "Game.h"
#include "ChatBox.h"

void process(RequestCoordinator*);
void tickInGame(WSServer* srv);

class WSServer : public websocketpp::server::handler {
public:
	static const int				IN_GAME_TICK = 80;
	static const int				NB_SIMULTANEOUS_GAMES=50;

	void							on_open(websocketpp::server::connection_ptr);
	void							on_message(websocketpp::server::connection_ptr,message_ptr);
	void							on_close(websocketpp::server::connection_ptr);
	std::string						get_con_id(const websocketpp::server::connection_ptr&);
	
									WSServer();
	virtual							~WSServer();
	RequestCoordinator*				getCoordinator(){return &coordinator;}
	
	ChatBox*						getChatBox(){return &chatBox;};
	std::string						getOutGameData(const websocketpp::server::connection_ptr&);
	bool							switchPlayerToGame(const websocketpp::server::connection_ptr&,const unsigned int&);
	void							createGame(const websocketpp::server::connection_ptr&);
	void							createPlayer(const websocketpp::server::connection_ptr&,const std::string&);
	void							createMessage(const std::string&,const std::string&,const std::string&,const std::string&,const std::string&);
	void							selectMapForGame(const unsigned short&,const unsigned short&,const websocketpp::server::connection_ptr&);
	void							closeConnection(const websocketpp::server::connection_ptr&);
	void							startGame(const unsigned short&,const websocketpp::server::connection_ptr&);
	void							chooseColor(const unsigned short&,const unsigned short&,const websocketpp::server::connection_ptr&);
	void							tickInGame();
	void							redirectMoveRequest(const unsigned short&,const unsigned short&,const websocketpp::server::connection_ptr&,const unsigned short&);
	void							redirectDropBombRequest(const unsigned short&,const unsigned short&,const websocketpp::server::connection_ptr&);
	void							addGameToStoppingGames(const unsigned short&);

private:
	std::list<Player*>				outGamePlayers;
	std::list<Game*> 				games;
	std::list<Game*>				runningGames;
	boost::mutex**					gameLockers;
	boost::mutex					stoppingGamesLocker;
	Game**							games_ptr;//tableau de pointeurs sur instances de Game, l'index est connu des clients pour optimiser la vitesse de traitement des requetes
	RequestCoordinator				coordinator;
	boost::mutex 					lockOutGamers;
	boost::mutex					lockInGamers;
	boost::mutex					lockOutGameMessages;
	RequestFactory*					requestFactory;
	ChatBox							chatBox;
	std::list<unsigned short>		stoppingGames;

	unsigned long					addNewGame(Player*);
	void							addNewPlayer(const websocketpp::server::connection_ptr&,const std::string&);
	void							removeGame(const unsigned short&);
	void							stopGame(const unsigned short&);

	void notifyPlayerJoined(const std::string&,const std::string&);
	void notifyMessageSent(const std::string&,const std::string&,const std::string&,const std::string&);
	void notifyGameCreated(const std::string&,const std::string&);
	void notifyPlayerExited(const std::string&);
	void notifyGameDeleted(const std::string&);
	void notifyError(const std::string&,const connection_ptr&);
	void notifyGameRemoved(const std::string&);
	void sendGameMapList(const connection_ptr&);
	void sendGameMapData(const connection_ptr&, const unsigned short&);
};

#endif /* WSSERVER_H_ */
