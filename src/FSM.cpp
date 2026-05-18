/*
 * FSM.cpp
 *
 *  Created on: May 3, 2018
 *      Author: VIDEOJUEGOS UTALCA
 */

#include "FSM.h"


//////////////////////////////FSMTransition//////////////////////
FSMTransition::~FSMTransition() {
}
void FSMTransition::onTransition(const GameState&){

}

//////////////////////////FSMState////////////////////////////
FSMState::FSMState(std::shared_ptr<Character> _character):character(_character) {
}

void FSMState::onEnter(const GameState& ) {
}

void FSMState::onExit(const GameState&  ) {
}

FSMState::~FSMState() {
}
void FSMState::addTransition(std::shared_ptr<FSMTransition> t){
	transitions.push_back(t);
}
std::shared_ptr<FSMTransition> FSMState::getActiveTransition(const GameState& gs){
	for(auto& t:transitions){
		if(t->isValid(gs)){
			return t;
		}
	}
	return std::shared_ptr<FSMTransition>();
}


/////////////////////////////FiniteStateMachine///////////////////////////
FiniteStateMachine::FiniteStateMachine(std::shared_ptr<Character> _character):character(_character) {
}

FiniteStateMachine::~FiniteStateMachine() {
}
