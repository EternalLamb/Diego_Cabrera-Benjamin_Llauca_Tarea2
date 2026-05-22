/*
 * Maze.h
 *
 *  Created on: Mar 23, 2018
 *      Author: VIDEOJUEGOS UTALCA
 */

#ifndef MAZE_H_
#define MAZE_H_
#include <vector>
#include <unordered_set>
#include "Node.h"
#include <algorithm>

class Maze {
	std::vector<Node> nodes;
	std::unordered_set<int> pills;
	std::unordered_set<int> powerPills;
	std::array<int,4> ghostStart;
	int pacmanStart;
	void loadMaze(const std::string &filename);
	bool internalPathExists(int from, int to, std::vector<bool> &visited) const;
public:
	Maze(const std::string &filename);
	virtual ~Maze();
	std::ostream& print(std::ostream &os) const;
	bool pathExists(int from, int to) const; //simple DFS
	std::vector<int> distancesFrom(int from) const; // dijkstra
	std::vector<Move> getShortestPath(int from, int to) const; // A star

	const std::array<int,4>& getGhostStart() const {
		return ghostStart;
	}

	int getPacmanStart() const {
		return pacmanStart;
	}
	int getNeighbour(int index, Move m) const{
		return nodes[index].getNeighbours()[m];
	}
	std::vector<std::pair<int,int>> getPillPositions()const{
		std::vector<std::pair<int,int>> pos;
		std::transform(pills.begin(),pills.end(),std::back_inserter(pos),[this](int pill){return nodes[pill].getPos();});
		return pos;
	}
	std::vector<std::pair<int,int>> getPowerPillPositions()const{
		std::vector<std::pair<int,int>> pos;
		std::transform(powerPills.begin(),powerPills.end(),std::back_inserter(pos),[this](int ppill){return nodes[ppill].getPos();});
		return pos;
	}
	std::pair<int, int> getNodePos(int index) const;
	bool hasPill(int index) const{return pills.find(index)!=pills.end();}
	void eatPill(int index){pills.erase(index);}
	bool hasPowerPill(int index) const{return powerPills.find(index)!=powerPills.end();}
	void eatPowerPill(int index){powerPills.erase(index);}
	bool isLegal(int index, Move m) const;
	bool isCrossRoad(int index) const;
	bool isCorner(int index) const;
	bool isDeadEnd(int index) const;
	bool isHallway(int index) const;
	int numNeighbours(int index) const;
	Move forcedGhostMove(int index, Move lastMove) const;
	std::vector<Move> getGhostLegalMoves(int index, Move lastMove) const;
	std::vector<Move> getPossibleMoves(int index) const;
	bool isDone() const;
};



#endif /* MAZE_H_ */
