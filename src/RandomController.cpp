/*
 * RandomController.cpp
 *
 *  Created on: Apr 23, 2018
 *      Author: nbarriga
 */

#include "RandomController.h"
#include <iostream>

RandomController::RandomController(std::shared_ptr<Character> character):
	Controller(character)
{
		srand(time(NULL));
}

RandomController::~RandomController() {
	// TODO Auto-generated destructor stub
}

Move
RandomController::getMove(const GameState& game){
	if(character->getDirection()==PASS){
		const auto moves=game.getMaze().getPossibleMoves(character->getPos());
		Move m=moves[rand()%moves.size()];
		return m;
	}
	const auto moves=game.getMaze().getGhostLegalMoves(character->getPos(),character->getDirection());
	Move m=moves[rand()%moves.size()];
	return m;
}
