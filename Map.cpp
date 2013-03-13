/*
 * Map.cpp
 *
 *  Created on: 9 déc. 2012
 *      Author: robin
 */

#include "Map.h"
#include "Utils.h"


/**
** Deux premieres composantes = width & height
** troisieme = nombre de joueurs
** 4&5emes,6&7emes,... = positions x,y de départ des joueurs
** Les suivantes représentent les données de map
**/
const std::string Map::maps[]={
	"15,13,4,1,1,13,1,1,11,13,11,12,14,14,14,14,14,14,14,14,14,14,14,14,14,16,11,2,2,2,2,2,2,2,2,2,2,2,2,2,9,11,2,10,2,10,2,10,2,10,2,10,2,10,2,9,11,2,2,2,2,2,2,2,2,2,2,2,2,2,9,11,2,10,2,10,2,10,2,10,2,10,2,10,2,9,11,2,2,2,2,2,2,2,2,2,2,2,2,2,9,11,2,10,2,10,2,10,2,10,2,10,2,10,2,9,11,2,2,2,2,2,2,2,2,2,2,2,2,2,9,11,2,10,2,10,2,10,2,10,2,10,2,10,2,9,11,2,2,2,2,2,2,2,2,2,2,2,2,2,9,11,2,10,2,10,2,10,2,10,2,10,2,10,2,9,11,2,2,2,2,2,2,2,2,2,2,2,2,2,9,8,6,6,6,6,6,6,6,6,6,6,6,6,6,4,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10"
							  };
Map::Map(){
	this->smap="0,0";
	this->init();
}

Map::Map(const std::string& smap){
	this->smap=smap;
	this->init();
}

std::string Map::getCurrentSMap()const{
	std::string s;
	std::ostringstream oss;
	oss<<this->width<<","<<this->height;
	for(int i = 0 ; i < this->width ; i++){
		for(int y = 0 ; y < this->height ; y++){
			s+=","+((unsigned short*)map[i])[y];
		}
	}

	return s;
}

void Map::setSMap(const std::string& smap){
	for(int i = 0 ; i < this->width ; i++){
		delete [] map[i];
		delete [] map;
	}

	this->smap=smap;
	this->init();
	if(this->playersPositions!=0)
		delete [] this->playersPositions;
}

void Map::init(){
	try{
		std::list<std::string> vec;
		Utils::split(vec,smap,',');
		if((this->width=atoi(vec.front().c_str()))==0)throw 1;vec.pop_front();
		if((this->height=atoi(vec.front().c_str()))==0)throw 1;vec.pop_front();
		if((this->nbPlayers=atoi(vec.front().c_str()))==0)throw 1;vec.pop_front();
		this->playersPositions=new Position[this->nbPlayers];

		for (int i = 0 ; i < this->nbPlayers ; i++){
			if((this->playersPositions[i].x=atoi(vec.front().c_str()))==0)throw 1;vec.pop_front();
			if((this->playersPositions[i].x=atoi(vec.front().c_str()))==0)throw 1;vec.pop_front();
		}

		this->map=new unsigned short*[width];
		for(int i=0;i<width;i++)this->map[i]=new unsigned short[height];

		for(int i = 0 ; i < this->width ; i++){
			for(int y = 0 ; y < this->height ; y++){
				((unsigned short*)map[i])[y]=atoi(vec.front().c_str());
				vec.pop_front();
			}
		}
		this->launchable=true;
	}catch(int e){
		this->launchable=false;
	}
}

Map::~Map() {
	for(int i = 0 ; i < this->width ; i++){
		delete [] map[i];
		delete [] map;
	}
}

bool Map::couldBeAPath(const unsigned short& x, const unsigned short& y){
	return this->map[x][y] == 2;
}