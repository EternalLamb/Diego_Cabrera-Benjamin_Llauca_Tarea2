#pragma once

#include "Controller.h"


class SueController: public Controller {

public:
	SueController(std::shared_ptr<Character> character);
	virtual ~SueController();
	virtual Move getMove(const GameState& game)override;
};

