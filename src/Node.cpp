/*
 * Node.cpp
 *
 *  Created on: Mar 23, 2018
 *      Author: VIDEOJUEGOS UTALCA
 */

#include "Node.h"
#include <algorithm>
#include <iterator>

Node::Node(int index_, int x_, int y_, std::array<int, 4> neighbours_):
			index(index_),x(x_),y(y_){

	std::copy(std::begin(neighbours_), std::end(neighbours_),std::begin(neighbours));
}
Node::Node():index(-1),x(-1),y(-1){}


Node::~Node() {
	// TODO Auto-generated destructor stub
}

std::ostream&
Node::print(std::ostream &os) const{
	os<<"Node Index: "<< index<<" ["<<x<<", "<<y<<"], neighbours: [";
	for(int i=0;i<4;i++){
		if(neighbours[i]!=-1){
			os<<neighbours[i]<<",";
		}
	}
	os<<"]";
	return os;
}

std::array<int,4>
Node::getNeighbours() const{
	return neighbours;
}

Move turnBack(Move m) {
	if (m == PASS)
		return PASS;

	return static_cast<Move>((m + 2) % 4);
}

Move turnLeft(Move m) {
	if (m == PASS)
		return PASS;

	return static_cast<Move>((m + 3) % 4);
}

Move turnRight(Move m) {
	if (m == PASS)
		return PASS;

	return static_cast<Move>((m + 1) % 4);
}
