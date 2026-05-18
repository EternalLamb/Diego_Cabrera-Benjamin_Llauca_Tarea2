#pragma once

#include "Controller.h"

class InkyController: public Controller {

    Move getClosestMove(const GameState& game, std::pair<int,int> target) const;
    Move getFarthestMove(const GameState& game, std::pair<int,int> target) const;
    
    float getDistanceToPacman(const GameState& game) const;
    
    Move getRandomMove(const GameState& game) const;

public:
    InkyController(std::shared_ptr<Character> character);
    virtual ~InkyController();
    virtual Move getMove(const GameState& game) override;
};