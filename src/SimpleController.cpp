/*
 * SimpleController.cpp
 *
 *  Created on: Apr 23, 2018
 *      Author: nbarriga
 */

#include "SimpleController.h"
#include <iostream>

SimpleController::SimpleController(std::shared_ptr<Character> character):
	Controller(character){
}

SimpleController::~SimpleController() {
	// TODO Auto-generated destructor stub
}

Move
SimpleController::getMove(const GameState& game){

	if(character->getDirection()==PASS && game.getMaze().getGhostStart()[0])	{
		return RIGHT;
	}
	
	int pacmanNode = game.getPacmanPos();
	auto pacmanCoords = game.getMaze().getNodePos(pacmanNode);
	
	
	int ghostNode = character->getPos();
	
	Ghost *ghost = dynamic_cast<Ghost*>(character.get());
	if(!ghost->isEdible()){
		int minDist=10000000;
		Move minMove=character->getDirection();
		std::vector<Move> moves=game.getMaze().getGhostLegalMoves(character->getPos(),character->getDirection());
		for(Move m:moves){

			int vecino = game.getMaze().getNeighbour(ghostNode,m);
			
			if(vecino<0)continue;
			auto vecinoCoords = game.getMaze().getNodePos(vecino);
			vecinoCoords.first-=pacmanCoords.first;
			vecinoCoords.second-=pacmanCoords.second;
			int sqDist=vecinoCoords.first*vecinoCoords.first+vecinoCoords.second*vecinoCoords.second;
			if(sqDist<minDist){
				minDist=sqDist;
				minMove=m;
			}
		}
		return minMove;
	}else{
		int maxDist=-1;
		Move maxMove=character->getDirection();
		std::vector<Move> moves=game.getMaze().getGhostLegalMoves(character->getPos(),character->getDirection());
		for(Move m:moves){
			int vecino = game.getMaze().getNeighbour(ghostNode,m);
			if(vecino<0)continue;
			auto vecinoCoords = game.getMaze().getNodePos(vecino);
			vecinoCoords.first-=pacmanCoords.first;
			vecinoCoords.second-=pacmanCoords.second;
			int sqDist=vecinoCoords.first*vecinoCoords.first+vecinoCoords.second*vecinoCoords.second;
			if(sqDist>maxDist){
				maxDist=sqDist;
				maxMove=m;
			}
		}
		return maxMove;
	}

	
}
