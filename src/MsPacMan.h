/*
 * MsPacMan.h
 *
 *  Created on: Apr 24, 2018
 *      Author: nbarriga
 */

#ifndef MSPACMAN_H_
#define MSPACMAN_H_

#include "Character.h"

class MsPacMan: public Character {
	int score;
	int lives;
public:
	MsPacMan(int startNode);
	virtual ~MsPacMan();
	virtual void die() override;
	void addScore(int points);
	int getScore()const;
	bool isDead() const;
	virtual void reset(int startNode) override;
};



#endif /* MSPACMAN_H_ */
