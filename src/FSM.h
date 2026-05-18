/*
 * FSM.h
 *
 *  Created on: May 2, 2018
 *      Author: VIDEOJUEGOS UTALCA
 */

#ifndef FSM_H_
#define FSM_H_
#include <list>
#include <memory>
#include "Character.h"
#include "GameState.h"

class FSMState;

class FSMTransition {
public:
	virtual bool isValid(const GameState& gs)=0 ;
	virtual std::shared_ptr<FSMState> getNextState()=0;
	virtual void onTransition(const GameState& gs);
	virtual ~FSMTransition();
};


class FSMState {

public:
	FSMState(std::shared_ptr<Character> _character);
	virtual void onEnter(const GameState& gs);
	virtual Move onUpdate(const GameState& gs)=0;
	virtual void onExit(const GameState& gs);
	virtual ~FSMState();
	void addTransition(std::shared_ptr<FSMTransition> t);
	std::shared_ptr<FSMTransition> getActiveTransition(const GameState& gs);
protected:
	std::shared_ptr<Character> character;
	std::list<std::shared_ptr<FSMTransition>> transitions;
};



class FiniteStateMachine {

public:
	FiniteStateMachine(std::shared_ptr<Character> _character);
	virtual Move update(const GameState& gs)=0;
	virtual ~FiniteStateMachine() ;
protected:
	std::shared_ptr<Character> character;
	std::list<std::shared_ptr<FSMState>> states;
	std::shared_ptr<FSMState> initialState;
	std::shared_ptr<FSMState> activeState;
};

#endif /* FSM_H_ */
