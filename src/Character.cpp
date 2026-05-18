/*
 * Character.cpp
 *
 *  Created on: Apr 17, 2018
 *      Author: VIDEOJUEGOS UTALCA
 */

#include "Character.h"

Character::Character(int startNode):currentNodeIndex(startNode),startNodeIndex(startNode),lastMove(PASS) {

}

Character::~Character() {
	// TODO Auto-generated destructor stub
}

int Character::move(Move m, const Maze& maze) {
	if(m==PASS){//if pass, then issue lastMove
		if(lastMove!=PASS &&maze.getNeighbour(currentNodeIndex,lastMove)!=-1){//if valid
			currentNodeIndex=maze.getNeighbour(currentNodeIndex, lastMove);
		}
	}else if(maze.getNeighbour(currentNodeIndex,m)!=-1){//if not a pass, issue move
		currentNodeIndex=maze.getNeighbour(currentNodeIndex, m);
		lastMove=m;
	}else{//invalid move, issue last Move
		if(lastMove!=PASS &&maze.getNeighbour(currentNodeIndex,lastMove)!=-1){//if valid
			currentNodeIndex=maze.getNeighbour(currentNodeIndex, lastMove);
		}
	}
	return currentNodeIndex;
}


Move Character::getDirection() const {
	return lastMove;
}

int Character::getPos() const {
	return currentNodeIndex;
}

void Character::die() {
	currentNodeIndex = startNodeIndex;
	lastMove = PASS;
}

void Character::reset(int startNode) {
	currentNodeIndex = startNodeIndex = startNode;
	lastMove = PASS;
}
