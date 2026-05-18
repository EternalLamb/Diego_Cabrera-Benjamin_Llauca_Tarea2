/*
 * Character.h
 *
 *  Created on: Apr 17, 2018
 *      Author: VIDEOJUEGOS UTALCA
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_
#include "GameState.h"
#include <memory>

class Controller {
protected:
	std::shared_ptr<Character> character;
public:
	Controller(std::shared_ptr<Character> character);
	virtual ~Controller();
	virtual Move getMove(const GameState& game) ;
};
float euclid2(std::pair<int,int> a,std::pair<int,int> b);
#endif /* CONTROLLER_H_ */
