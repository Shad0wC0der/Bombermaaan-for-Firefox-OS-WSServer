/*
 * Map.h
 *
 *  Created on: 9 déc. 2012
 *      Author: robin
 */

#ifndef MAP_H_
#define MAP_H_
#include <sstream>
#include "Utils.h"

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

private:
	unsigned short width,
				   height;
	unsigned short** map;
	std::string smap;

	void init();

};

#endif /* MAP_H_ */
