#pragma once

#include "Controller.h"


class BlinkyController: public Controller {

public:
	BlinkyController(std::shared_ptr<Character> character);
	virtual ~BlinkyController();
	virtual Move getMove(const GameState& game)override;
};

