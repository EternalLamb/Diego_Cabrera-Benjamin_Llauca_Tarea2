#include "PacmanController.h"
#include <SDL2/SDL.h>

PacmanController::PacmanController(std::shared_ptr<Character> character):
	Controller(character){
}

PacmanController::~PacmanController() {
	// TODO Auto-generated destructor stub
}

Move getOppositeDirection(Move m) {
    if (m == UP) return DOWN;
    if (m == DOWN) return UP;
    if (m == LEFT) return RIGHT;
    if (m == RIGHT) return LEFT;
    return PASS;
}

Move PacmanController::getClosestMove(const GameState& game, std::pair<int,int> target)const{
	int minDist=10000000;
	Move minMove=character->getDirection();
	std::vector<Move> moves=game.getMaze().getPossibleMoves(character->getPos());
	for(Move m:moves){
		int vecino = game.getMaze().getNeighbour(character->getPos(),m);
		if(vecino<0)continue;
		auto vecinoCoords = game.getMaze().getNodePos(vecino);
		int sqDist=euclid2(vecinoCoords,target);
		if(sqDist<minDist){
			minDist=sqDist;
			minMove=m;
		}
	}
	return minMove;
}

Move PacmanController::getFarthestMove(const GameState& game, std::pair<int,int> target)const{
	int maxDist=-1;
	Move maxMove=character->getDirection();
	std::vector<Move> moves=game.getMaze().getPossibleMoves(character->getPos());
	for(Move m:moves){
		int vecino = game.getMaze().getNeighbour(character->getPos(),m);
		if(vecino<0)continue;
		auto vecinoCoords = game.getMaze().getNodePos(vecino);
		int sqDist=euclid2(vecinoCoords,target);
		if(sqDist>maxDist){
			maxDist=sqDist;
			maxMove=m;
		}
	}
	return maxMove;
}

float PacmanController::getDistanceToGhost(const GameState& game, int g)const{
	return sqrt(euclid2(
		game.getMaze().getNodePos(character->getPos()),
		game.getMaze().getNodePos(game.getGhostsPos(g))));
}

Move
PacmanController::getMove(const GameState& game){

	//para cerrar la ventana
	SDL_Event e;
	if( SDL_PollEvent( &e ) != 0 )
	{
		if( e.type == SDL_QUIT || 
			(e.type == SDL_KEYDOWN && 
				(e.key.keysym.sym==SDLK_ESCAPE || 
				e.key.keysym.sym==SDLK_q) ))
		{
			SDL_Quit();
			exit(0);
		}
	}
	
	int pacmanNode = character->getPos();
	auto pacmanCoords = game.getMaze().getNodePos(pacmanNode);
	Move currentDir = character->getDirection();
	
	std::vector<std::pair<int,int>> ghostPositions;
	for(int i=0;i<4;i++){
		ghostPositions.push_back(game.getMaze().getNodePos(game.getGhostsPos(i)));
	}

	std::vector<bool> ghostsEdible;
	for(int i=0;i<4;i++){
		ghostsEdible.push_back(game.isGhostEdible(i));
	}

	auto powerPillPositions=game.getMaze().getPowerPillPositions();	


	float fear=0.0f;
	Move escapeMove = PASS;
	float hunger=0.0f;
	Move eatGhostMove = PASS;



	//arrancar de fantasmas cercanos que me pueden comer 
	for(int i=0;i<4;i++){
		if((!ghostsEdible[i])){
			//cambio de formula para que el miedo sea menor
			float tempFear=1.0f-1.0f/(1.0f+pow(2.718f * 0.45f,-getDistanceToGhost(game,i)+20.0f));//logistica
			if(tempFear>fear){
				fear=tempFear;
				escapeMove=getFarthestMove(game,ghostPositions[i]);
			}
		}
		
	}

	//perseguir fantasmas azules
	for(int i=0;i<4;i++){
		if(ghostsEdible[i]){
			//cambio de formula para que el hambre sea mayor
			float tempHunger= 1.5f * (pow(100.0f-getDistanceToGhost(game,i),2)/pow(100.0,2));//cuadratica
			if(tempHunger>hunger){
				hunger=tempHunger;
				eatGhostMove=getClosestMove(game,ghostPositions[i]);;
			}
		}
	}

	Move finalMove = PASS;

	if(fear > hunger) {
        finalMove = escapeMove;
    } else if (hunger > 0.0f) {
        finalMove = eatGhostMove;
    }

	//obliga a mantener la direccion elegida para evitar que oscile entre 2 o mas direcciones
	Move oppositeDir = getOppositeDirection(currentDir);
    if (finalMove == oppositeDir && fear < 0.85f) {
        finalMove = PASS;
    }

	std::vector<Move> possibleMoves = game.getMaze().getPossibleMoves(pacmanNode);
    if (possibleMoves.size() > 1) { // Es un pasillo continuo o una intersección ??
        if (finalMove == oppositeDir) {
            for (Move m : possibleMoves) {
                if (m != oppositeDir && game.getMaze().getNeighbour(pacmanNode, m) >= 0) {
                    finalMove = m; // Desvia a Pac-Man hacia un camino alternativo antes que retroceder
                    break;
                }
            }
        }
    }

	if (game.getMaze().getNeighbour(pacmanNode, finalMove) < 0 && !possibleMoves.empty()) {
        finalMove = possibleMoves[0];
    }

    return finalMove;
}
