/*
 * KeyboardController.h
 *
 *  Created on: Apr 17, 2018
 *      Author: VIDEOJUEGOS UTALCA
 */

#ifndef KEYBOARDCONTROLLER_H_
#define KEYBOARDCONTROLLER_H_

#include "Controller.h"
#include <SDL2/SDL.h>

class KeyboardController: public Controller {
	Move lastMove;
public:
	KeyboardController(std::shared_ptr<Character> character);
	virtual ~KeyboardController();
	virtual Move getMove(const GameState& game) override;
};

#endif /* KEYBOARDCONTROLLER_H_ */
