#include "SueController.h"


SueController::SueController(std::shared_ptr<Character> character):
	Controller(character){
}

SueController::~SueController() {

}

Move
SueController::getMove(const GameState& game){
	return PASS;	

	
}
