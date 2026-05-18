/*
 * Character.cpp
 *
 *  Created on: Apr 17, 2018
 *      Author: VIDEOJUEGOS UTALCA
 */

#include "Controller.h"
#include <iostream>
Controller::Controller(std::shared_ptr<Character> _character):character(_character) {
	// TODO Auto-generated constructor stub

}

Controller::~Controller() {
	// TODO Auto-generated destructor stub
}

Move Controller::getMove(const GameState&) {
	//std::cerr<<"controller getmove \n";
	return PASS;
}
float euclid2(std::pair<int,int> a,std::pair<int,int> b){
	return (a.first-b.first)*(a.first-b.first) +(a.second-b.second)*(a.second-b.second);
}