/*
 * Player.h
 *
 *  Created on: 9 déc. 2012
 *      Author: robin
 */

#ifndef PLAYER_H_
#define PLAYER_H_

class Player {
public:
	Player();
	virtual ~Player();
	std::string getName();
	void setname(std::string);
private:
	std::string color;
	std::string name;
};

#endif /* PLAYER_H_ */
