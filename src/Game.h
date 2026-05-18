/*
 * Game.h
 *
 *  Created on: Apr 17, 2018
 *      Author: VIDEOJUEGOS UTALCA
 */

#ifndef GAME_H_
#define GAME_H_

#include "GameState.h"
#include "Controller.h"
#include "GameView.h"
#include <memory>

class Game {
	int currentMap;
	std::vector<std::string> filenames;
	GameState gameState;
	std::vector<std::shared_ptr<Controller>> ghostsControl;
	std::shared_ptr<Controller> pacmanControl;
	std::unique_ptr<GameView> gv;
public:
	Game();
	void run();
};

#endif /* GAME_H_ */
