/*
 * RandomController.h
 *
 *  Created on: Apr 23, 2018
 *      Author: nbarriga
 */

#ifndef RANDOMCONTROLLER_H_
#define RANDOMCONTROLLER_H_

#include "Controller.h"
#include <random>

class RandomController: public Controller {
public:
	RandomController(std::shared_ptr<Character> character);
	virtual ~RandomController();
	virtual Move getMove(const GameState& game)override;
};

#endif /* RANDOMCONTROLLER_H_ */
