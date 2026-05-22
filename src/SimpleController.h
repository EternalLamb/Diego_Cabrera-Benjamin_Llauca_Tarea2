/*
 * SimpleController.h
 *
 *  Created on: Apr 23, 2018
 *      Author: nbarriga
 */

#ifndef SIMPLECONTROLLER_H_
#define SIMPLECONTROLLER_H_

#include "Controller.h"


class SimpleController: public Controller {

public:
	SimpleController(std::shared_ptr<Character> character);
	virtual ~SimpleController();
	virtual Move getMove(const GameState& game)override;
};

#endif /* SIMPLECONTROLLER_H_ */
