/*
 * GameState.cpp
 *
 *  Created on: Apr 17, 2018
 *      Author: VIDEOJUEGOS UTALCA
 */

#include "GameState.h"
#include <cassert>
#include <iostream>

GameState::GameState(const std::string &filename):
maze(filename){

}
void GameState::addPacMan(std::shared_ptr<MsPacMan> _pacman) {
	pacman=_pacman;
}

void GameState::addGhosts(const std::vector<std::shared_ptr<Ghost> >& _ghosts) {
	ghosts=_ghosts;
}

GameState::~GameState() {
	// TODO Auto-generated destructor stub
}

void GameState::updatePacman(Move m) {
	int newPos=pacman->move(m,maze);

	if(maze.hasPill(newPos)){
		maze.eatPill(newPos);
		pacman->addScore(10);
	}
	if(maze.hasPowerPill(newPos)){
		maze.eatPowerPill(newPos);
		pacman->addScore(50);
		for(auto &ghost:ghosts){
			ghost->setEdible();
		}
	}
}


void GameState::updateGhosts(std::vector<Move> m) {
	assert(ghosts.size()==m.size());
	for(unsigned int i=0;i<ghosts.size();i++){
		auto legal= getMaze().getGhostLegalMoves(ghosts[i]->getPos(),ghosts[i]->getDirection());
		if((ghosts[i]->getDirection()==PASS) || std::count(legal.begin(),legal.end(),m[i])!=0){
			ghosts[i]->move(m[i],maze);
		}
		else{
			//std::cout<<"Ghost "<<i<<" issued an illegal move!\n";
			ghosts[i]->move(PASS,maze);
		}
		
		ghosts[i]->update();
	}
}
void GameState::updateEaten(){
	for(unsigned int i=0;i<ghosts.size();i++){
		if(ghosts[i]->getPos()==pacman->getPos()){
			if(ghosts[i]->isEdible()){
				ghosts[i]->die();
				pacman->addScore(400);
			}else{
				pacman->die();
			}
		}
	}
}
bool GameState::won() const {
	if (maze.isDone()) {
		return true;
	}
	return false;
}

bool GameState::lost() const {
	if (pacman->isDead()) {
		return true;
	}
	return false;
}

void GameState::reset(std::string map) {
	maze = Maze(map);
	pacman->reset(maze.getPacmanStart());
	for (unsigned int i = 0; i < ghosts.size(); i++) {
		ghosts[i]->reset(maze.getGhostStart()[i]);
	}
}
