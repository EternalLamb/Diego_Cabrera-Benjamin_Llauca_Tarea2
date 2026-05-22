#ifndef SIMPLEPACMANCONTROLLER_H_
#define SIMPLEPACMANCONTROLLER_H_

#include "Controller.h"


class SimplePacmanController: public Controller {
	Move getClosestMove(const GameState& game, std::pair<int,int> target)const;
	Move getFarthestMove(const GameState& game, std::pair<int,int> target)const;
	float getDistanceToGhost(const GameState& game, int g)const;
public:
	SimplePacmanController(std::shared_ptr<Character> character);
	virtual ~SimplePacmanController();
	virtual Move getMove(const GameState& game)override;
};

#endif /* SIMPLEPACMANCONTROLLER_H_ */
