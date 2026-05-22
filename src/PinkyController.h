#pragma once

#include "BehaviorTree.h"
#include "Controller.h"
#include <chrono>

#include <random>
class Pinky_Info{
    static Pinky_Info *info;
    Pinky_Info(){}

public:
    static Pinky_Info* getInfo(){
        if(info==nullptr)info = new Pinky_Info();
        return info;
    }
    const GameState* in_gamestate;
    Move out_move;
    std::shared_ptr<Character> in_character;
};



class PinkyController: public Controller {

private:
	std::shared_ptr<Composite> root;
public:
	PinkyController(std::shared_ptr<Character> character);
	virtual ~PinkyController();
	virtual Move getMove(const GameState& gs)override;
};


class Chase_Pinky : public Behavior{
public:
    virtual Status update() override;

};

class Frightened_Pinky : public Behavior{
private:
    std::mt19937 e;
    std::uniform_int_distribution<int> uniform_dist;
public:
    virtual Status update() override;
    Frightened_Pinky();

};

class Scatter_Pinky : public Behavior{
private:
    std::pair<int,int> target;

public:
    virtual Status update() override;
    Scatter_Pinky();

};

class Powerpill_Pinky : public Behavior{
public:
    virtual Status update() override;
};

class TimeOut_Pinky	 : public Behavior{
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> lastTime;
public:
    virtual Status update() override;
    TimeOut_Pinky();
};