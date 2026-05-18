/*
 * Ghost.h
 *
 *  Created on: Apr 24, 2018
 *      Author: nbarriga
 */

#ifndef GHOST_H_
#define GHOST_H_

#include "Character.h"
#include <chrono>

class Ghost: public Character {
	bool edible;
	std::chrono::system_clock::time_point turnedEdible;
public:
	Ghost(int startNode);
	virtual ~Ghost();

	bool isEdible() const {
		return edible;
	}

	void setEdible();
	void revert();
	void update();

	virtual void die() override;
	virtual void reset(int startNode) override;
};



#endif /* GHOST_H_ */
