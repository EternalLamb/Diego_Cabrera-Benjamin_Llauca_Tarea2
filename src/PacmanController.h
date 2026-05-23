#pragma once

#include "Controller.h"


class PacmanController: public Controller {
	Move getClosestMove(const GameState& game, std::pair<int,int> target)const;
	Move getFarthestMove(const GameState& game, std::pair<int,int> target)const;
	float getDistanceToGhost(const GameState& game, int g)const;
	Move getExploreMove(const GameState& game) const;
	Move getEscapeMoveFromAll(const GameState& game, const std::vector<std::pair<int,int>>& threats) const;
public:
	PacmanController(std::shared_ptr<Character> character);
	virtual ~PacmanController();
	virtual Move getMove(const GameState& game)override;
};
