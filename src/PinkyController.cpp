#include "PinkyController.h"
#include <iostream>

Pinky_Info* Pinky_Info::info=nullptr;


PinkyController::PinkyController(std::shared_ptr<Character> character):Controller(character),root(std::make_shared<Selector>()){
	auto filter = std::make_shared<Filter>();
	filter->addCondition(std::make_shared<Powerpill_Pinky>());
	filter->addAction(std::make_shared<Frightened_Pinky>());
	root->addChild(filter);
	auto filter2 = std::make_shared<Filter>();
	filter2->addCondition(std::make_shared<TimeOut_Pinky>());
	filter2->addAction(std::make_shared<Scatter_Pinky>());
	root->addChild(filter2);
	root->addChild(std::make_shared<Chase_Pinky>());
}

PinkyController::~PinkyController() {

}

Move
PinkyController::getMove(const GameState& gs){
	Pinky_Info::getInfo()->in_character=character;
	Pinky_Info::getInfo()->in_gamestate=&gs;
	root->tick();

	return Pinky_Info::getInfo()->out_move;	
}

TimeOut_Pinky::TimeOut_Pinky() : Behavior() {
	lastTime = std::chrono::high_resolution_clock::now();

}

Status TimeOut_Pinky::update(){
	std::chrono::duration<float> timeStamp = std::chrono::high_resolution_clock::now() - lastTime;
	if( (int)timeStamp.count()%27 < 7){
		return BH_SUCCESS;
	}else{
		return BH_FAILURE;
	}

}

Status Chase_Pinky::update(){
	//std::cerr << " Chase \n" ;
	auto character = Pinky_Info::getInfo()->in_character;
	auto gs = Pinky_Info::getInfo()->in_gamestate;
	auto target= gs->getMaze().getNodePos(gs->getPacmanPos());
	float min=1000000000;
	Move minMove=PASS;
	std::vector<Move> moves;
	if(character->getDirection()==PASS) {
		moves=gs->getMaze().getPossibleMoves(character->getPos());
	} else {
		moves = gs->getMaze().getGhostLegalMoves(character->getPos(), character->getDirection());
	}

	for(auto move:moves) {
		if(move==PASS) {
			break;
		}
		float dist = euclid2(target,gs->getMaze().getNodePos(gs->getMaze().getNeighbour(character->getPos(),move)));
		if(dist<min) {
			min=dist;
			minMove=move;
		}
	}
	Pinky_Info::getInfo()->out_move = minMove;
	return BH_SUCCESS;
}

Status Powerpill_Pinky::update(){
	auto character = Pinky_Info::getInfo()->in_character;
	auto ghost = dynamic_cast<Ghost*>(character.get());

	if( ghost!=nullptr && ghost->isEdible()){
		return BH_SUCCESS;
	}else{
		return BH_FAILURE;
	}

}

Frightened_Pinky::Frightened_Pinky() : Behavior(), e(rand()), uniform_dist(0,3){

}

Status Frightened_Pinky::update(){
	//std::cerr << " Frightened \n" ;
	auto character = Pinky_Info::getInfo()->in_character;
	auto gs = Pinky_Info::getInfo()->in_gamestate;
	std::vector<Move> moves;
	if(character->getDirection()==PASS) {
		moves=gs->getMaze().getPossibleMoves(character->getPos());
	} else {
		moves = gs->getMaze().getGhostLegalMoves(character->getPos(), character->getDirection());
	}
	Move m = moves[rand()%moves.size()];
	Pinky_Info::getInfo()->out_move = m;
	return BH_SUCCESS; //NO es as� pero por ahora
}

Scatter_Pinky :: Scatter_Pinky() : Behavior(){
	target = std::make_pair(-1,-1);

}

Status Scatter_Pinky::update(){
	//std::cerr << " Scatter \n" ;
	if(target.first == -1){
		target = Pinky_Info::getInfo()->in_gamestate->getMaze().getPowerPillPositions()[0];
	}

	auto character = Pinky_Info::getInfo()->in_character;
	auto gs = Pinky_Info::getInfo()->in_gamestate;

	Move minMove=PASS;
	std::vector<Move> moves;
	if(character->getDirection()==PASS) {
		moves=gs->getMaze().getPossibleMoves(character->getPos());
	} else {
		moves = gs->getMaze().getGhostLegalMoves(character->getPos(), character->getDirection());
	}

	float min=100000000;
	for(auto move:moves) {
		if(move==PASS) {
			break;
		}
		float dist = euclid2(target,gs->getMaze().getNodePos(gs->getMaze().getNeighbour(character->getPos(),move)));
		if(dist<min) {
			min=dist;
			minMove=move;
		}
	}
	Pinky_Info::getInfo()->out_move = minMove;
	return BH_SUCCESS;

}
