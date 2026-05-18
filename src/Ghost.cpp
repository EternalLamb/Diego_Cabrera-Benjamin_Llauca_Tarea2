/*
 * Ghost.cpp
 *
 *  Created on: Apr 24, 2018
 *      Author: nbarriga
 */

#include "Ghost.h"


Ghost::Ghost(int startNode):Character(startNode),edible(false) {
	// TODO Auto-generated constructor stub

}

Ghost::~Ghost() {
	// TODO Auto-generated destructor stub
}

void Ghost::setEdible() {
	this->edible = true;
	turnedEdible = std::chrono::system_clock::now();
	revert();
}
void Ghost::revert(){
	switch(lastMove){
		case UP:
			lastMove=DOWN;
			break;
		case DOWN:
			lastMove=UP;
			break;
		case LEFT:
			lastMove=RIGHT;
			break;
		case RIGHT:
			lastMove=LEFT;
			break;
		case PASS:
			break;
	}
}
void Ghost::update() {
	if ((std::chrono::system_clock::now() - turnedEdible)
			> std::chrono::seconds(5)) {
		edible = false;
	}
}

void Ghost::die(){
	Character::die();
	edible=false;
}
void Ghost::reset(int startNode)
{
	Character::reset(startNode);
	edible = false;
}
