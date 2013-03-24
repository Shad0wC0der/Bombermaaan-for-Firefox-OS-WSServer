/*
 * Utils.h
 *
 *  Created on: 9 déc. 2012
 *      Author: robin
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <list>
#include <string>

namespace Utils{
	int split(std::list<std::string>& vecteur, std::string chaine, char separateur)
	{
		vecteur.clear();
		std::string::size_type stTemp = chaine.find(separateur);
		while(stTemp != std::string::npos)
		{
			vecteur.push_back(chaine.substr(0, stTemp));
			chaine = chaine.substr(stTemp + 1);
			stTemp = chaine.find(separateur);
		}
		vecteur.push_back(chaine);
		return vecteur.size();
	}
}
#endif /* UTILS_H_ */