/*
 * Node.h
 *
 *  Created on: Mar 23, 2018
 *      Author: VIDEOJUEGOS UTALCA
 */

#ifndef NODE_H_
#define NODE_H_
#include <array>
#include <iostream>

enum Move:unsigned char { UP, RIGHT, DOWN, LEFT, PASS};
Move turnRight(Move m);
Move turnLeft(Move m);
Move turnBack(Move m);

class Node {
	int index;
	std::array<int,4> neighbours;  //node indices of neighbours
	int x, y;			//for drawing
public:
	Node(int index, int x, int y, std::array<int,4> neighbours);
	Node();
	virtual ~Node();
	std::ostream& print(std::ostream &os) const;
	std::array<int,4> getNeighbours() const;
	int getNeighbour(Move m) const{return neighbours[m];};
	std::pair<int,int> getPos()const{
		return std::make_pair(x,y);
	}
};

#endif /* NODE_H_ */
