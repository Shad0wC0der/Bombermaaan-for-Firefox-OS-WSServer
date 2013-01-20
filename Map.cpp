/*
 * Map.cpp
 *
 *  Created on: 9 déc. 2012
 *      Author: robin
 */

#include "Map.h"

const std::string Map::maps[]={
	"15,15,12,14,14,14,14,14,14,14,14,14,14,14,14,14,16,11,2,2,2,2,2,2,2,2,2,2,2,2,2,9,11,2,10,2,10,2,10,2,10,2,10,2,10,2,9,11,2,2,2,2,2,2,2,2,2,2,2,2,2,9,11,2,10,2,10,2,10,2,10,2,10,2,10,2,9,11,2,2,2,2,2,2,2,2,2,2,2,2,2,9,11,2,10,2,10,2,10,2,10,2,10,2,10,2,9,11,2,2,2,2,2,2,2,2,2,2,2,2,2,9,11,2,10,2,10,2,10,2,10,2,10,2,10,2,9,11,2,2,2,2,2,2,2,2,2,2,2,2,2,9,11,2,10,2,10,2,10,2,10,2,10,2,10,2,9,11,2,2,2,2,2,2,2,2,2,2,2,2,2,9,8,6,6,6,6,6,6,6,6,6,6,6,6,6,4,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10"
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
		for(int y = 0 ; y < this->height ; y++){
			delete &((unsigned short*)map[i])[y];
		}
	}
	this->smap=smap;
	this->init();
}

void Map::init(){
	std::list<std::string> vec;

	split(vec,smap,',');

	this->width=atoi(vec.front().c_str());vec.pop_front();
	this->height=atoi(vec.front().c_str());vec.pop_front();

	this->map=new unsigned short*[width];
	for(int i=0;i<width;i++)this->map[i]=new unsigned short[height];

	for(int i = 0 ; i < this->width ; i++){
		for(int y = 0 ; y < this->height ; y++){
			((unsigned short*)map[i])[y]=atoi(vec.front().c_str());
			vec.pop_front();
		}
	}
}

Map::~Map() {
	for(int i = 0 ; i < this->width ; i++){
		for(int y = 0 ; y < this->height ; y++){
			delete &((unsigned short*)map[i])[y];
		}
	}
}
