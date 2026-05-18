/*
 * Maze.cpp
 *
 *  Created on: Mar 23, 2018
 *      Author: VIDEOJUEGOS UTALCA
 */

#include "Maze.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
extern bool nogui;

Maze::Maze(const std::string &filename) {
	loadMaze(filename);
}

Maze::~Maze() {
	// TODO Auto-generated destructor stub
}

void
Maze::loadMaze(const std::string &filename){
/*
First line:

start node for Ms Pac-Man
4 start nodes for the ghosts

Other lines:

node index
x-coordinate
y-coordinate
neighbouring node in UP direction (-1 if none)
neighbouring node in RIGHT direction (-1 if none)
neighbouring node in DOWN direction (-1 if none)
neighbouring node in LEFT direction (-1 if none)
pill-index of the node (-1 if none)
power-pill index of the node (-1 if none)

*/
	if(!nogui)
		std::cout<<"Loading level: "<<filename<<std::endl;
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr<<"failed to open " +filename<<std::endl;
		throw new std::runtime_error("failed to open " +filename);
	}
	file>>pacmanStart;
	for(auto &i:ghostStart){
		file>>i;
	}
	while(!file.eof()){
		unsigned int index;
		int x, y;
		std::array<int,4> neighbours;
		file>>index>>x>>y;

		for(auto &i:neighbours){
			file>>i;
		}

		int pill, powerPill;
		file>>pill>>powerPill;

		//grow and then copy
//		if(index>=nodes.size())
//			nodes.resize(index+1);
//		nodes[index]=Node(index,x,y,neighbours);


		//alternative, use emplace
		if(index!=nodes.size()){
			std::cerr<<"unordered nodes!"<<std::endl;
			throw new std::runtime_error("unordered nodes!");
		}
		nodes.emplace(nodes.end(),index,x,y,neighbours);

		if(pill!=-1){
			pills.insert(index);
		}
		if(powerPill!=-1){
			powerPills.insert(index);
		}

	}

	file.close();


}

std::ostream&
Maze::print(std::ostream &os) const{
	for(const auto &n:nodes){
		n.print(os);
		os<<std::endl;
	}
	return os;
}

bool Maze::internalPathExists(int from, int to, std::vector<bool> &visited) const{
	if(from==to)return true;

	for(auto n:nodes[from].getNeighbours()){
		if(n==-1||visited[n])continue;
		visited[n]=true;
		if(n==to)return true;
		if(internalPathExists(n,to,visited)){
			return true;
		}

	}
	return false;
}

bool
Maze::pathExists(int from, int to) const{
	auto visited=std::vector<bool>(nodes.size(),false);
	return internalPathExists(from,to,visited);

}

bool Maze::isLegal(int index, Move m) const {
	if (m == PASS) {
		return true;
	}
	if (nodes[index].getNeighbour(m) != -1) {
		return true;
	}
	return false;
}

bool Maze::isCrossRoad(int index) const {
	return numNeighbours(index)>2;
}

std::pair<int, int> Maze::getNodePos(int index) const {
	return nodes[index].getPos();
}

Move Maze::forcedGhostMove(int index, Move lastMove) const {
	if(isCorner(index)){//take corner
		Move m=turnRight(lastMove);
		if(isLegal(index,m)){
			return m;
		}else{
			m=turnLeft(lastMove);
			if(isLegal(index,m)){
				return m;
			}else{
				throw new std::runtime_error("corner, but can't turn either way");//shouldn't happen!
			}
		}
	}
	if(isHallway(index)){//continue
		return lastMove;
	}
	if(isDeadEnd(index)){//go back
		return turnBack(lastMove);
	}
	return PASS;//no forced move
}

bool Maze::isHallway(int index) const {
	if(numNeighbours(index)==2){
		if ((getNeighbour(index, UP) != -1 && getNeighbour(index, DOWN) != -1) ||
				(getNeighbour(index, LEFT) != -1 && getNeighbour(index, RIGHT) != -1)) {
			return true;
		}
	}
	return false;

}

std::vector<Move> Maze::getGhostLegalMoves(int index, Move lastMove) const {
	std::vector<Move> moves;
	if(isCrossRoad(index)){
		for(auto dir:{UP,DOWN,LEFT,RIGHT}){
			if(isLegal(index,dir)&&dir!=turnBack(lastMove)){
				moves.push_back(dir);
			}
		}
		if(moves.empty())
			moves.push_back(PASS);
	}else{
		moves.push_back(forcedGhostMove(index,lastMove));
	}
	return moves;
}

std::vector<Move> Maze::getPossibleMoves(int index) const {
	std::vector<Move> moves;
	for(auto dir:{UP,DOWN,LEFT,RIGHT}){
		if(isLegal(index,dir)){
			moves.push_back(dir);
		}
	}
	if(moves.empty())moves.push_back(PASS);
	return moves;
}

bool Maze::isDone() const {
	if (powerPills.size() + pills.size() == 0) {
		return true;
	}
	return false;
}

bool Maze::isCorner(int index) const {
	if(numNeighbours(index)==2){
		bool NS = false, EW = false;
		if (getNeighbour(index, UP) != -1 || getNeighbour(index, DOWN) != -1) {
			NS = true;
		}
		if (getNeighbour(index, LEFT) != -1 || getNeighbour(index, RIGHT) != -1) {
			EW = true;
		}
		return EW && NS;
	}else{
		return false;
	}
}

bool Maze::isDeadEnd(int index) const {
	return numNeighbours(index)==1;
}

int Maze::numNeighbours(int index) const {
	int num=0;
	for(auto dir:{UP,DOWN,LEFT,RIGHT}){
		if(getNeighbour(index,dir)!=-1){
			num++;
		}
	}
	return num;
}
