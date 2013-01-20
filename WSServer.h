/*
 * WSServer.h
 *
 *  Created on: 9 déc. 2012
 *      Author: robin
 */

#ifndef WSSERVER_H_
#define WSSERVER_H_
#include <sstream>
#include "RequestCoordinator.h"
#include "Game.h"
#include "ChatBox.h"
#include "RequestClose.h"

void process(RequestCoordinator*);
void tickInGame();
class WSServer : public websocketpp::server::handler {
public:
	static const int				IN_GAME_TICK = 80;
	static const int				NB_SIMULTANEOUS_GAMES=50;
	void							on_open(connection_ptr);
	void							on_message(connection_ptr,message_ptr);
	void							on_close(connection_ptr);
	std::string						get_con_id(const connection_ptr&);
	
									WSServer();
	virtual							~WSServer();
	RequestCoordinator*				getCoordinator(){return &coordinator;}
	
	ChatBox*						getChatBox(){return &chatBox;};
	std::string						getOutGameData();
	bool							switchPlayerToGame(const std::string&,const unsigned int&);
	void							createGame(const connection_ptr&);
	void							createPlayer(const connection_ptr&,const std::string&);
	void							createMessage(const std::string&,const std::string&,const std::string&,const std::string&,const std::string&);
	void							closeConnection(const connection_ptr&);
private:
	std::list<Player*>				outGamePlayers;
	std::list<Game*> 				games;
	std::list<Game*>				runningGames;
	boost::mutex**					gameLockers;
	Game**							games_ptr;//tableau de pointeurs sur instances de Game, l'index est connu des clients pour optimiser la vitesse de traitement des requetes
	RequestCoordinator				coordinator;
	boost::mutex 					lockOutGamers;
	boost::mutex					lockInGamers;
	boost::mutex					lockOutGameMessages;
	RequestFactory*					requestFactory;
	ChatBox							chatBox;

	unsigned long					addNewGame(Player*);
	void							addNewPlayer(const connection_ptr&,const std::string&);
	void							removeGame(const unsigned short&);
	void							startGame(const unsigned short&);
	void							stopGame(const unsigned short&);

	void notifyPlayerJoined(const std::string&,const std::string&);
	void notifyMessageSent(const std::string&,const std::string&,const std::string&,const std::string&);
	void notifyGameCreated(const std::string&,const std::string&);
	void notifyPlayerExited(const std::string&);
	void notifyGameDeleted(const std::string&);
	void notifyError(const std::string&,const connection_ptr&);
	void notifyEnteringRoom(Player*,Game*);
	void notifyExitingRoom(const std::string&,Game*);
	void notifyGameRemoved(const std::string&);
	void sendGameMapList(const connection_ptr&);
	void sendGameMapData(const connection_ptr&, const unsigned short&);
	void notifyGameStarted(Game*);
	void notifyGameFinished(Game*);
};

#endif /* WSSERVER_H_ */
