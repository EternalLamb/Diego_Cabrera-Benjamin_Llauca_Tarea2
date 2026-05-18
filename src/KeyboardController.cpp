/*
 * KeyboardController.cpp
 *
 *  Created on: Apr 17, 2018
 *      Author: VIDEOJUEGOS UTALCA
 */

#include "KeyboardController.h"

#include <iostream>

KeyboardController::KeyboardController(std::shared_ptr<Character> _character):Controller(_character) {
	lastMove=PASS;
}

KeyboardController::~KeyboardController() {
	// TODO Auto-generated destructor stub
}

Move KeyboardController::getMove(const GameState&)  {

	SDL_Event e;
	//Handle events on queue
	while( SDL_PollEvent( &e ) != 0 )
	{
		//User requests quit
		if( e.type == SDL_QUIT )
		{
			SDL_Quit();
			exit(0);
		}
		//User presses a key
		else if( e.type == SDL_KEYDOWN )
		{
			//Select surfaces based on key press
			switch( e.key.keysym.sym )
			{
			case SDLK_w:
				lastMove = UP;
				break;
			case SDLK_s:
				lastMove = DOWN;
				break;
			case SDLK_a:
				lastMove = LEFT;
				break;
			case SDLK_d:
				lastMove = RIGHT;
				break;
			case SDLK_ESCAPE:
				SDL_Quit();
				exit(0);
			}
		}
	}

	return lastMove;
}
