/*
 * Character.h
 *
 *  Created on: Apr 17, 2018
 *      Author: VIDEOJUEGOS UTALCA
 */

#ifndef CHARACTER_H_
#define CHARACTER_H_
#include "Maze.h"

class Character {
protected:
	int currentNodeIndex,startNodeIndex;
	Move lastMove;
public:
	Character(int startNode);
	virtual ~Character();
	int move(Move m, const Maze& maze);

	Move getDirection() const;
	int getPos() const;
	virtual void die();
	virtual void reset(int startNode);
	friend class GameView;
};


#endif /* CHARACTER_H_ */
