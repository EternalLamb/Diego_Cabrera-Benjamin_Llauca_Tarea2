/*
 * GameState.h
 *
 *  Created on: Apr 17, 2018
 *      Author: VIDEOJUEGOS UTALCA
 */

#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include <vector>

#include "Character.h"
#include "Ghost.h"
#include "MsPacMan.h"
#include "Maze.h"
#include <memory>

class GameState {
	Maze maze;
	std::shared_ptr<MsPacMan> pacman;
	std::vector<std::shared_ptr<Ghost>> ghosts;


public:
	GameState(const std::string &filename);
	void addPacMan(std::shared_ptr<MsPacMan> pacman);
	void addGhosts(const std::vector<std::shared_ptr<Ghost>> &ghosts);
//	GameState(const std::string &filename,
//			std::shared_ptr<Character> pacman,
//			const std::vector<std::shared_ptr<Character>> &ghosts);
	virtual ~GameState();
	void updatePacman(Move m);
	void updateGhosts(std::vector<Move> m);
	void updateEaten();
	int getScore() const{
		return pacman->getScore();
	}
	const Maze& getMaze() const {
		return maze;
	}
	int isGhostEdible(int g) const{
		return ghosts[g]->isEdible();
	}
	int getGhostsPos(int g) const {
		return ghosts[g]->getPos();
	}
	int getPacmanPos() const {
		return pacman->getPos();
	}
	int getGhostsDir(int g) const {
		return ghosts[g]->getDirection();
	}
	int getPacmanDir() const {
		return pacman->getDirection();
	}
	bool won() const;
	bool lost() const;
	void reset(std::string map);
	friend class GameView;
};



#endif /* GAMESTATE_H_ */
