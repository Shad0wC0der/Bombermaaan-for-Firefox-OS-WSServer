/*
 * Game.cpp
 *
 *  Created on: 9 déc. 2012
 *      Author: robin
 */
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "Game.h"
#include "WSServer.h"

const unsigned short Game::NB_COLORS = 4;

Game::Game(/*std::string name,*/Player* host,const unsigned short& id,WSServer* server) {
	this->id = id;
	//this->name=name;
	this->host=host;
	this->addPlayer(host);
	this->map.setSMap(Map::maps[0]);
	this->server=server;
	this->inGamePlayers=new std::pair<Player*,InGamePlayerData>[MAX_PLAYER];
	this->nbPlayers=1;
}

bool Game::tryToRemovePlayerByCon(const websocketpp::server::connection_ptr& con){
	bool hostExited=false;
	for(int i = 0 ; i < this->nbPlayers ; i++){
		if(inGamePlayers[i].first->getCon() == con){
			if(this->host == inGamePlayers[i].first)hostExited=true;
			delete inGamePlayers[i].first;
			//reordonancement
			//si la suppression cause un trou, on decalle
			if(i<this->nbPlayers-1){
				for(int p = i ; p < this->nbPlayers-1 ; p++){
					inGamePlayers[p]=inGamePlayers[p+1];	
				}
			}
			this->nbPlayers--;
			if(hostExited && !this->nbPlayers==0)
				host=inGamePlayers[0].first;
			return true;
		}
	}
	return false;
}

Game::~Game() {
	// TODO Auto-generated destructor stub
}

bool Game::addPlayer(Player* player){
	if(this->nbPlayers<MAX_PLAYER){
		InGamePlayerData inGamePlayerData;
		inGamePlayerData.color=PLAYER_COLOR::COLOR_NONE;
		inGamePlayerData.speed=20;
		inGamePlayerData.radius=2;
		inGamePlayerData.maxBomb=1;
		inGamePlayerData.bombUsed=0;
		inGamePlayerData.alive=true;
		this->inGamePlayers[this->nbPlayers++]=std::pair<Player*,InGamePlayerData>(player,inGamePlayerData);
		return true;
	}
	return false;
}

bool Game::isColorAvalaible(const unsigned short& color){
	if(color > NB_COLORS)
		return false;

	for(int i = 0 ; i < this->nbPlayers ; i++){
		if(inGamePlayers[i].second.color == color)
			return false;
	}

	return true;
}

bool Game::isInGame(const websocketpp::server::connection_ptr& con){
	for(int i = 0 ; i < this->nbPlayers ; i++){
		if(inGamePlayers[i].first->getCon()==con)
		return true;
	}
	return false;
}

PLAYER_COLOR Game::getColor(const unsigned short& color){
	switch(color){
	case 1:
		return PLAYER_COLOR::WHITE;break;
	case 2:
		return PLAYER_COLOR::BLACK;break;
	case 3:
		return PLAYER_COLOR::RED;break;
	case 4:
		return PLAYER_COLOR::BLUE;break;
	default:
		return PLAYER_COLOR::COLOR_NONE;break;
	}
}

void Game::startGame(){
	//map
	if (this->map.getBlocks() == 0)
		this->map.setSMap(Map::maps[0]);

	//initialisation players
	for(int i = 0 ; i < this->nbPlayers ; i++){
		inGamePlayers[i].second.position.x=this->map.getPlayerPosition(i).x;
		inGamePlayers[i].second.position.y=this->map.getPlayerPosition(i).y;
	}

	//initialisation aleatoire des blocs
	this->blocks=new bool*[this->map.getWidth()];
	for(int i=0;i<this->map.getWidth();i++)this->blocks[i]=new bool[this->map.getHeight()];

	for(int y = 0 ; y < this->map.getWidth() ; y++) {
		for(int x = 0 ; x < this->map.getHeight() ; x++) {
			if (this->map.getBlocks()[y][x] == 2) {
				bool free=true;	
				for(int i = 0 ; i < this->nbPlayers ; i++){
					if (!(fabs((double)(inGamePlayers[i].second.position.x-x)) > 1 || fabs((double)(inGamePlayers[i].second.position.y-y)) > 1)){
						free=false;
						break;
					}
				}
				
				if (free) {					
					if (floor (rand() % 10 + 1) > 1) {
						this->blocks[x][y]=true;
					}else{
						this->blocks[x][y]=false;
					}
				}	 
			}
		}
	}

	//intialisation des obstructions par blocs
	this->bombObstructions=new bool*[this->map.getWidth()];
	for(int i=0;i<this->map.getWidth();i++)this->bombObstructions[i]=new bool[this->map.getHeight()];
	for(int i = 0 ; i < this->map.getWidth() ; i++){
		for(int y = 0 ; y < this->map.getHeight() ; y++){
			((bool*)bombObstructions[i])[y]=false;
		}
	}

	//intialisation de la couche item
	this->items=new unsigned short*[this->map.getWidth()];
	for(int i=0;i<this->map.getWidth();i++)this->items[i]=new unsigned short[this->map.getHeight()];
	for(int i = 0 ; i < this->map.getWidth() ; i++){
		for(int y = 0 ; y < this->map.getHeight() ; y++){
			((unsigned short*)items[i])[y]=0;
		}
	}

	//initialisation des MOVE de chaque joueurs
	for(int i = 0 ; i < this->nbPlayers ; i++){
		this->moves[i].timer=255;
		this->moves[i].endTime=255;
	}

	//initialisation deflagrations 
	this->deflagrations=new unsigned short*[this->map.getWidth()];
	for(int i=0;i<this->map.getWidth();i++)this->deflagrations[i]=new unsigned short[this->map.getHeight()];
	for(int i = 0 ; i < this->map.getWidth() ; i++){
		for(int y = 0 ; y < this->map.getHeight() ; y++){
			((unsigned short*)deflagrations[i])[y]=0;
		}
	}

	this->notifyGameStarted();
}

void Game::stopGame(){
	this->server->stopGame(this->id);
}

void Game::dropBomb(const unsigned short& iPlayer,const websocketpp::server::connection_ptr& con){
	if(iPlayer<0 || iPlayer>=this->nbPlayers) return;
	if(inGamePlayers[iPlayer].first->getCon() != con) return;
	if(!(inGamePlayers[iPlayer].second.bombUsed<inGamePlayers[iPlayer].second.maxBomb)) return;
	if(!inGamePlayers[iPlayer].second.alive) return;
	if(this->bombObstructions[inGamePlayers[iPlayer].second.position.x][inGamePlayers[iPlayer].second.position.y])return;

	Bomb b;
	b.radius=inGamePlayers[iPlayer].second.radius;
	b.endTime=BOMB_TIMER;
	b.timer=0;
	b.position.x=inGamePlayers[iPlayer].second.position.x;
	b.position.y=inGamePlayers[iPlayer].second.position.y;
	inGamePlayers[iPlayer].second.bombUsed++;

	this->bombs.push_back(b);
	this->notifyBombDropped(iPlayer,b);
}

void Game::move(const unsigned short& iPlayer,const websocketpp::server::connection_ptr& con,const unsigned short& direction){
	if(iPlayer<0 || iPlayer>=this->nbPlayers) return;
	if(inGamePlayers[iPlayer].first->getCon() != con) return ;
	if(moves[iPlayer].timer<moves[iPlayer].endTime) return;
	if(!inGamePlayers[iPlayer].second.alive) return;

	switch(direction){
		case DIRECTION::UP:
			if(this->isObstructed(inGamePlayers[iPlayer].second.position.x,inGamePlayers[iPlayer].second.position.y-1))return;
			moves[iPlayer].direction=DIRECTION::UP;
			break;
		case DIRECTION::RIGHT:
			if(this->isObstructed(inGamePlayers[iPlayer].second.position.x+1,inGamePlayers[iPlayer].second.position.y))return;
			moves[iPlayer].direction=DIRECTION::RIGHT;
			break;
		case DIRECTION::DOWN:
			if(this->isObstructed(inGamePlayers[iPlayer].second.position.x,inGamePlayers[iPlayer].second.position.y+1))return;
			moves[iPlayer].direction=DIRECTION::DOWN;
			break;
		case DIRECTION::LEFT:
			if(this->isObstructed(inGamePlayers[iPlayer].second.position.x-1,inGamePlayers[iPlayer].second.position.y))return;
			moves[iPlayer].direction=DIRECTION::LEFT;
			break;
		default:
			return;
	}

	moves[iPlayer].endTime=inGamePlayers[iPlayer].second.speed;
	moves[iPlayer].timer=0;
	moves[iPlayer].tChangePosition=(moves[iPlayer].endTime/2);
	this->notifyMove(iPlayer,moves[iPlayer]);
}

Player* Game::getPlayer(const unsigned short& iPlayer){
	if(iPlayer<0 || !(iPlayer<this->nbPlayers))return (Player*)0;
	return this->inGamePlayers[iPlayer].first;
}

void Game::changePlayerColor(const websocketpp::server::connection_ptr& con,const unsigned short& color){
	if(!isColorAvalaible(color))return;
	for(int i = 0 ; i < this->nbPlayers ; i++){
		if(inGamePlayers[i].first->getCon() == con){
			inGamePlayers[i].second.color=(PLAYER_COLOR)color;
		}
	}
}

void Game::notifyEnteringRoom(Player* player){
	std::ostringstream oss;
	oss<<getID();
	std::string m = "{\"type\":\""+std::string(stringify(NOTIFY_ENTERING_ROOM))+"\",\"value\":{\"name\":\""+player->getName()+"\",\"id\":\""+player->getID()+"\",\"gameID\":\""+oss.str()+"\"}}";
	for(int i = 0 ; i < this->nbPlayers ; i++){
		inGamePlayers[i].first->getCon()->send(m);
	}
}

void Game::notifyExitingRoom(const std::string& playerID){
	std::string m = "{\"type\":\""+std::string(stringify(NOTIFY_EXITING_ROOM))+"\",\"value\":{\"playerID\":\""+playerID+"\"}}";
	for(int i = 0 ; i < this->nbPlayers ; i++){
		inGamePlayers[i].first->getCon()->send(m);
	}
}


void Game::notifyGameStarted(){
	std::ostringstream oss;
	oss<<getID();
	std::string m = "{\"type\":\""+std::string(stringify(NOTIFY_GAME_STARTED))+"\",\"value\":{\"gameID\":\""+oss.str()+"\"}}";
	for(int i = 0 ; i < this->nbPlayers ; i++){
		inGamePlayers[i].first->getCon()->send(m);
	}
}

void Game::notifyGameFinished(){
	std::ostringstream oss;
	oss<<getID();
	std::string m = "{\"type\":\""+std::string(stringify(NOTIFY_GAME_FINISHED))+"\",\"value\":{\"gameID\":\""+oss.str()+"\"}}";
	for(int i = 0 ; i < this->nbPlayers ; i++){
		inGamePlayers[i].first->getCon()->send(m);
	}
}

void Game::notifyColorChanged(){
	if(this->nbPlayers == 0) return;

	std::ostringstream oss1;
	oss1<<getID();
	std::string m = "{\"type\":\""+std::string(stringify(NOTIFY_COLOR_CHANGED))+"\",\"value\":{\"gameID\":\""+oss1.str()+"\",\"colors\":[";
	
	std::ostringstream oss2;
	oss2<<inGamePlayers[0].second.color;
	m+="\"playerID\":\""+inGamePlayers[0].first->getID()+"\",\"color\":\""+oss2.str()+"\"";

	for(unsigned short i = 1 ; i < this->nbPlayers ; i++){
		std::ostringstream oss3;
		oss3<<inGamePlayers[0].second.color;
		m+=",\"playerID\":\""+inGamePlayers[i].first->getID()+"\",\"color\":\""+oss3.str()+"\"";
	}

	m+="]}}";

	for(unsigned short i = 0 ; i < this->nbPlayers ; i++){
		inGamePlayers[i].first->getCon()->send(m);
	}
	delete inGamePlayers;
}

void Game::notifyMove(const unsigned short& iPlayer,const Move& move){
	std::ostringstream oss1,oss2,oss3;
	oss1<<getID();
	oss2<<iPlayer;
	oss3<<move.direction;
	std::string m = "{\"type\":\""+std::string(stringify(NOTIFY_MOVE))+"\",\"value\":{\"gameID\":\""+oss1.str()+"\",\"playerSlot\":\""+oss2.str()+"\",\"direction\":\""+oss3.str()+"\"}}";
	for(int i = 0 ; i < this->nbPlayers ; i++){
		inGamePlayers[i].first->getCon()->send(m);
	}
}

void Game::notifyBombDropped(const unsigned short& iPlayer,const Bomb& bomb){
	std::ostringstream oss1,oss2,oss3,oss4,oss5;
	oss1<<getID();
	oss2<<iPlayer;
	oss3<<bomb.position.x;
	oss4<<bomb.position.y;
	oss5<<bomb.radius;
	std::string m = "{\"type\":\""+std::string(stringify(NOTIFY_BOMB_DROPPED))+"\",\"value\":{\"gameID\":\""+oss1.str()+"\",\"playerSlot\":\""+oss2.str()+"\",\"x\":\""+oss3.str()+"\",\"y\":\""+oss4.str()+"\",\"radius\":\""+oss5.str()+"\"}}";
	for(int i = 0 ; i < this->nbPlayers ; i++){
		inGamePlayers[i].first->getCon()->send(m);
	}
}

void Game::notifyBlockDestroyed(const Position& position){
	std::ostringstream oss1,oss2;
	oss1<<position.x;
	oss2<<position.y;
	std::string m = "{\"type\":\""+std::string(stringify(NOTIFY_BLOCK_DESTROYED))+"\",\"value\":{\"x\":\""+oss1.str()+"\",\"y\":\""+oss2.str()+"\"}}";
	for(int i = 0 ; i < this->nbPlayers ; i++){
		inGamePlayers[i].first->getCon()->send(m);
	}
}

void Game::notifyBombExploded(const Position& position){
	std::ostringstream oss1,oss2;
	oss1<<position.x;
	oss2<<position.y;
	std::string m = "{\"type\":\""+std::string(stringify(NOTIFY_BOMB_EXPLODED))+"\",\"value\":{\"x\":\""+oss1.str()+"\",\"y\":\""+oss2.str()+"\"}}";
	for(int i = 0 ; i < this->nbPlayers ; i++){
		inGamePlayers[i].first->getCon()->send(m);
	}
}

void Game::notifyItemAppeared(const unsigned short& itemType, const Position& position){
	std::ostringstream oss1,oss2,oss3;
	oss1<<itemType;
	oss2<<position.x;
	oss3<<position.y;
	std::string m = "{\"type\":\""+std::string(stringify(NOTIFY_ITEM_APPEARED))+"\",\"value\":{\"itemType\":\""+oss1.str()+"\",\"x\":\""+oss2.str()+"\",\"y\":\""+oss3.str()+"\"}}";
	for(int i = 0 ; i < this->nbPlayers ; i++){
		inGamePlayers[i].first->getCon()->send(m);
	}
}

void Game::notifyBonusAcquired(const unsigned short& iPlayer, const unsigned short& itemType){
	std::ostringstream oss1,oss2;
	oss1<<iPlayer;
	oss2<<itemType;
	std::string m = "{\"type\":\""+std::string(stringify(NOTIFY_BONUS_ACQUIRED))+"\",\"value\":{\"playerSlot\":\""+oss1.str()+"\",\"itemType\":\""+oss2.str()+"\"}}";
	for(int i = 0 ; i < this->nbPlayers ; i++){
		inGamePlayers[i].first->getCon()->send(m);
	}
}

void Game::notifyItemPickedup(const Position& position){
	std::ostringstream oss1,oss2;
	oss1<<position.x;
	oss2<<position.y;
	std::string m = "{\"type\":\""+std::string(stringify(NOTIFY_ITEM_PICKEDUP))+"\",\"value\":{\"x\":\""+oss1.str()+"\",\"y\":\""+oss2.str()+"\"}}";
	for(int i = 0 ; i < this->nbPlayers ; i++){
		inGamePlayers[i].first->getCon()->send(m);
	}
}

void Game::notifyItemDestroyed(const Position& position){
	std::ostringstream oss1,oss2;
	oss1<<position.x;
	oss2<<position.y;
	std::string m = "{\"type\":\""+std::string(stringify(NOTIFY_ITEM_DESTROYED))+"\",\"value\":{\"x\":\""+oss1.str()+"\",\"y\":\""+oss2.str()+"\"}}";
	for(int i = 0 ; i < this->nbPlayers ; i++){
		inGamePlayers[i].first->getCon()->send(m);
	}
}

void Game::notifyPlayerKilled(const unsigned short& iPlayer){
	std::ostringstream oss1;
	oss1<<iPlayer;
	std::string m = "{\"type\":\""+std::string(stringify(NOTIFY_PLAYER_KILLED))+"\",\"value\":{\"playerSlot\":\""+oss1.str()+"\"}}";
	for(int i = 0 ; i < this->nbPlayers ; i++){
		inGamePlayers[i].first->getCon()->send(m);
	}
}

void Game::perform(){
	//verification fin de partie
	if(this->isGameFinished()){
		this->notifyGameFinished();
		this->stopGame();
	}

	//mouvements
	for(int i = 0 ; i < this->nbPlayers ; i++){
		if(moves[i].timer<moves[i].endTime){
			moves[i].timer++;
			if(moves[i].timer == moves[i].tChangePosition){
				switch(moves[i].direction){
				case DIRECTION::UP:
					inGamePlayers[i].second.position.y--;
					break;
				case DIRECTION::RIGHT:
					inGamePlayers[i].second.position.x++;
					break;
				case DIRECTION::DOWN:
					inGamePlayers[i].second.position.y++;
					break;
				case DIRECTION::LEFT:
					inGamePlayers[i].second.position.x--;
					break;
				default:
					return;
				}
				//verification acquisition bonus
				this->checkBonusAcquisition(i);
			}

			//verification mort du personnage
			if(checkDeath(i)){
				inGamePlayers[i].second.alive=false;
				this->notifyPlayerKilled(i);
			}
		}
	}

	//bombes
	for(Bomb b : this->bombs){
		if(b.timer==b.endTime){
			//deflagration
			this->doDeflagration(b);
		}else{
			b.timer++;
		}
	}

	//deflagrations
	for(int x = 0 ; x < this->map.getWidth() ; x++){
		for(int y = 0 ; y < this->map.getHeight() ; y++){
			if(this->deflagrations[x][y]>0){
				this->deflagrations[x][y]--;
			}
		}
	}

}

bool Game::isObstructed(const unsigned short& x,const unsigned short& y){
	return (!this->map.couldBeAPath(x,y)
			|| this->blocks[x][y]
			|| this->bombObstructions[x][y]);
}

void Game::doDeflagration(const Bomb& bomb){
	//suppression de la bombe
	this->bombs.remove(bomb);
	
	//notification d'explosion
	this->notifyBombExploded(bomb.position);

	//centre
	this->bombObstructions[bomb.position.x][bomb.position.y]=false;
	this->deflagrations[bomb.position.x][bomb.position.y]=DEFLAGRATION_TIMER;
			
	//haut
	for(int i = 1 ; i < bomb.radius ; i++){
		if(this->map.couldBeAPath(bomb.position.x,bomb.position.y - i)){
			//atteint une autre bombe
			if(this->bombObstructions[bomb.position.x][bomb.position.y - i]){
				for(Bomb b : bombs){
					if(b.position.x == bomb.position.x && b.position.y == bomb.position.y - i){
						this->doDeflagration(b);
						break;
					}
				}
			//atteint un bloc destructible
			}else if (this->blocks[bomb.position.x][bomb.position.y - i]){
				this->blocks[bomb.position.x][bomb.position.y - i]=false;
				Position p;
				p.x=bomb.position.x;
				p.y=bomb.position.y - i;
				this->notifyBlockDestroyed(p);
				int r = floor (rand() % 10 + 1);
				switch(r){
					case 0:
					case 1:
					case 2:
						this->items[bomb.position.x][bomb.position.y - i] = ITEM_TYPE::BOOT;
						this->notifyItemAppeared(ITEM_TYPE::BOOT,p);
						break;
					case 3:
					case 4:
						this->items[bomb.position.x][bomb.position.y - i] = ITEM_TYPE::BOMB;
						this->notifyItemAppeared(ITEM_TYPE::BOMB,p);
						break;
					case 5:
						this->items[bomb.position.x][bomb.position.y - i] = ITEM_TYPE::POWER;
						this->notifyItemAppeared(ITEM_TYPE::POWER,p);
						break;
				}
				break;
			//atteint un item
			}else if (this->items[bomb.position.x][bomb.position.y - i] != ITEM_TYPE::ITEM_NONE){
				//item détruis
				this->items[bomb.position.x][bomb.position.y - i] = ITEM_TYPE::ITEM_NONE;
				Position p;
				p.x=bomb.position.x;
				p.y=bomb.position.y - i;
				this->notifyItemDestroyed(p);
			}
			this->deflagrations[bomb.position.x][bomb.position.y - i]=DEFLAGRATION_TIMER;
		}
	}

	//droite
	for(int i = 1 ; i < bomb.radius ; i++){
		if(this->map.couldBeAPath(bomb.position.x + i,bomb.position.y)){
			//atteint une autre bombe
			if(this->bombObstructions[bomb.position.x + i][bomb.position.y]){
				for(Bomb b : bombs){
					if(b.position.x == bomb.position.x + i && b.position.y == bomb.position.y){
						this->doDeflagration(b);
						break;
					}
				}
			//atteint un bloc destructible
			}else if (this->blocks[bomb.position.x + i][bomb.position.y]){
				this->blocks[bomb.position.x + i][bomb.position.y]=false;
				Position p;
				p.x=bomb.position.x + i;
				p.y=bomb.position.y;
				this->notifyBlockDestroyed(p);
				int r = floor (rand() % 10 + 1);
				switch(r){
					case 0:
					case 1:
					case 2:
						this->items[bomb.position.x + i][bomb.position.y] = ITEM_TYPE::BOOT;
						this->notifyItemAppeared(ITEM_TYPE::BOOT,p);
						break;
					case 3:
					case 4:
						this->items[bomb.position.x + i][bomb.position.y] = ITEM_TYPE::BOMB;
						this->notifyItemAppeared(ITEM_TYPE::BOMB,p);
						break;
					case 5:
						this->items[bomb.position.x + i][bomb.position.y] = ITEM_TYPE::POWER;
						this->notifyItemAppeared(ITEM_TYPE::POWER,p);
						break;
				}
				break;
			//atteint un item
			}else if (this->items[bomb.position.x + i][bomb.position.y] != ITEM_TYPE::ITEM_NONE){
				//item détruis
				this->items[bomb.position.x + i][bomb.position.y] = ITEM_TYPE::ITEM_NONE;
				Position p;
				p.x=bomb.position.x + i;
				p.y=bomb.position.y;
				this->notifyItemDestroyed(p);
			}
			this->deflagrations[bomb.position.x + 1][bomb.position.y]=DEFLAGRATION_TIMER;
		}
	}
	//bas
	for(int i = 1 ; i < bomb.radius ; i++){
		if(this->map.couldBeAPath(bomb.position.x,bomb.position.y + i)){
			//atteint une autre bombe
			if(this->bombObstructions[bomb.position.x][bomb.position.y + i]){
				for(Bomb b : bombs){
					if(b.position.x == bomb.position.x && b.position.y == bomb.position.y + i){
						this->doDeflagration(b);
						break;
					}
				}
			//atteint un bloc destructible
			}else if (this->blocks[bomb.position.x][bomb.position.y + i]){
				this->blocks[bomb.position.x][bomb.position.y + i]=false;
				Position p;
				p.x=bomb.position.x;
				p.y=bomb.position.y + i;
				this->notifyBlockDestroyed(p);
				int r = floor (rand() % 10 + 1);
				switch(r){
					case 0:
					case 1:
					case 2:
						this->items[bomb.position.x][bomb.position.y + i] = ITEM_TYPE::BOOT;
						this->notifyItemAppeared(ITEM_TYPE::BOOT,p);
						break;
					case 3:
					case 4:
						this->items[bomb.position.x][bomb.position.y + i] = ITEM_TYPE::BOMB;
						this->notifyItemAppeared(ITEM_TYPE::BOMB,p);
						break;
					case 5:
						this->items[bomb.position.x][bomb.position.y + i] = ITEM_TYPE::POWER;
						this->notifyItemAppeared(ITEM_TYPE::POWER,p);
						break;
				}
				break;
			//atteint un item
			}else if (this->items[bomb.position.x][bomb.position.y + i] != ITEM_TYPE::ITEM_NONE){
				//item détruis
				this->items[bomb.position.x][bomb.position.y + i] = ITEM_TYPE::ITEM_NONE;
				Position p;
				p.x=bomb.position.x;
				p.y=bomb.position.y + i;
				this->notifyItemDestroyed(p);
			}
			this->deflagrations[bomb.position.x][bomb.position.y + i]=DEFLAGRATION_TIMER;
		}
	}
	//gauche
	for(int i = 1 ; i < bomb.radius ; i++){
		if(this->map.couldBeAPath(bomb.position.x - i,bomb.position.y)){
			//atteint une autre bombe
			if(this->bombObstructions[bomb.position.x - i][bomb.position.y]){
				for(Bomb b : bombs){
					if(b.position.x == bomb.position.x - i && b.position.y == bomb.position.y){
						this->doDeflagration(b);
						break;
					}
				}
			//atteint un bloc destructible
			}else if (this->blocks[bomb.position.x - i][bomb.position.y]){
				this->blocks[bomb.position.x - i][bomb.position.y]=false;
				Position p;
				p.x=bomb.position.x - i;
				p.y=bomb.position.y;
				this->notifyBlockDestroyed(p);
				int r = floor (rand() % 10 + 1);
				switch(r){
					case 0:
					case 1:
					case 2:
						this->items[bomb.position.x - i][bomb.position.y] = ITEM_TYPE::BOOT;
						this->notifyItemAppeared(ITEM_TYPE::BOOT,p);
						break;
					case 3:
					case 4:
						this->items[bomb.position.x - i][bomb.position.y] = ITEM_TYPE::BOMB;
						this->notifyItemAppeared(ITEM_TYPE::BOMB,p);
						break;
					case 5:
						this->items[bomb.position.x - i][bomb.position.y] = ITEM_TYPE::POWER;
						this->notifyItemAppeared(ITEM_TYPE::POWER,p);
						break;
				}
				break;
			//atteint un item
			}else if (this->items[bomb.position.x - i][bomb.position.y] != ITEM_TYPE::ITEM_NONE){
				//item détruis
				this->items[bomb.position.x - i][bomb.position.y] = ITEM_TYPE::ITEM_NONE;
				Position p;
				p.x=bomb.position.x - i;
				p.y=bomb.position.y;
				this->notifyItemDestroyed(p);
			}
			this->deflagrations[bomb.position.x - i][bomb.position.y]=DEFLAGRATION_TIMER;
		}
	}
}

void Game::checkBonusAcquisition(const unsigned short& iPlayer){
	if(this->items[inGamePlayers[iPlayer].second.position.x][inGamePlayers[iPlayer].second.position.y] != ITEM_TYPE::ITEM_NONE){
		switch (this->items[inGamePlayers[iPlayer].second.position.x][inGamePlayers[iPlayer].second.position.y])
		{
		case ITEM_TYPE::BOOT:
			if(inGamePlayers[iPlayer].second.speed>8)
				inGamePlayers[iPlayer].second.speed--;
			break;
		case ITEM_TYPE::BOMB:
			inGamePlayers[iPlayer].second.maxBomb++;
			break;
		case ITEM_TYPE::POWER:
			inGamePlayers[iPlayer].second.radius++;
			break;
		}
		this->notifyBonusAcquired(iPlayer,this->items[inGamePlayers[iPlayer].second.position.x][inGamePlayers[iPlayer].second.position.y]);
		this->items[inGamePlayers[iPlayer].second.position.x][inGamePlayers[iPlayer].second.position.y] = ITEM_TYPE::ITEM_NONE;
		this->notifyItemPickedup(inGamePlayers[iPlayer].second.position);
	}
}

bool Game::checkDeath(const unsigned short& iPlayer){
	return (deflagrations[inGamePlayers[iPlayer].second.position.x][inGamePlayers[iPlayer].second.position.y] > 0);
}

bool Game::isGameFinished(){
	int cpt = 0 ; 
	for(int i = 0 ; i < this->nbPlayers ; i++){
		if(inGamePlayers[i].second.alive)
			cpt++;
	}

	return cpt < 2;
}
