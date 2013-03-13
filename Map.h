/*
 * Map.h
 *
 *  Created on: 9 déc. 2012
 *      Author: robin
 */

#ifndef MAP_H_
#define MAP_H_
#include <sstream>

struct Position{
	unsigned short int x;
	unsigned short int y;
};

class Map {
public:
	static const std::string maps[];
	static const unsigned short NB_SERVER_MAP=1;
	Map();
	Map(const std::string&);
	virtual ~Map();

	std::string getSMap()const{return this->smap;}
	void setSMap(const std::string&);
	std::string getCurrentSMap()const;
	unsigned short getWidth()const{return this->width;}
	unsigned short getHeight()const{return this->height;}
	Position getPlayerPosition(const int& index)const{return this->playersPositions[index];};
	unsigned short ** getBlocks()const{return this->map;}
	bool couldBeAPath(const unsigned short& x, const unsigned short& y);

private:
	unsigned short width,
				   height,
				   nbPlayers;
	Position*	   playersPositions;
	unsigned short** map;
	std::string smap;
	void init();
	bool launchable;
};

#endif /* MAP_H_ */
