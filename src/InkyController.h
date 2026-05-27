#pragma once

#include "BehaviorTree.h"
#include "Controller.h"
#include <chrono>
#include <random>

class Inky_Info{
    static Inky_Info* info;
    Inky_Info(){}
public:
    static Inky_Info* getInfo(){
        if(info==nullptr) info = new Inky_Info();
        return info;
    }
    const GameState* in_gamestate;
    Move out_move;
    std::shared_ptr<Character> in_character;
};

class InkyController : public Controller{
private:
    std::shared_ptr<Composite> root;
public:
    InkyController(std::shared_ptr<Character> Character);
    virtual ~InkyController();
    virtual Move getMove(const GameState& gs) override;
};

class PowerPill_Inky : public Behavior{
public:
    virtual Status update() override;
};
class OutOfRange_Inky : Behavior{
public:
    static constexpr float RADIO = 40.0f;
    virtual Status update() override;
}
class Frightened_Inky : public Behavior{
public:
    virtual Status update() override;
};
class Wander_Inky : public Behavior{
public:
    virtual Status update() override;
};
class Chase_Inky : public Behavior{
public:
    virtual Status update() override;
};