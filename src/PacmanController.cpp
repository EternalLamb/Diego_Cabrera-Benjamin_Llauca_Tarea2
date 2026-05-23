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
Move PacmanController::getEscapeMoveFromAll(const GameState& game, const std::vector<std::pair<int,int>>& threats) const {
    std::vector<Move> moves = game.getMaze().getPossibleMoves(character->getPos());
    Move currentDir = character->getDirection();
    Move opposite    = getOppositeDirection(currentDir);

    Move bestMove  = currentDir;
    int  bestScore = -1;

    for (Move m : moves) {
        // Héroe: no retrocede salvo que sea la única opción
        if (m == opposite && moves.size() > 1) continue;

        int vecino = game.getMaze().getNeighbour(character->getPos(), m);
        if (vecino < 0) continue;

        auto vecinoCoords = game.getMaze().getNodePos(vecino);

        // Suma de distancias al cuadrado a TODOS los fantasmas amenazantes
        int totalDist = 0;
        for (auto& t : threats)
            totalDist += euclid2(vecinoCoords, t);

        if (totalDist > bestScore) {
            bestScore = totalDist;
            bestMove  = m;
        }
    }
    return bestMove;
}
Move PacmanController::getExploreMove(const GameState& game) const {
    int pacmanNode   = character->getPos();
    auto pacmanCoords = game.getMaze().getNodePos(pacmanNode);
    Move currentDir  = character->getDirection();
    Move opposite    = getOppositeDirection(currentDir);

    auto pillPositions      = game.getMaze().getPillPositions();
    auto powerPillPositions = game.getMaze().getPowerPillPositions();

    std::vector<std::pair<int,int>> allPills;
    allPills.insert(allPills.end(), pillPositions.begin(),      pillPositions.end());
    allPills.insert(allPills.end(), powerPillPositions.begin(), powerPillPositions.end());

    if (allPills.empty()) return PASS;

    // Pill cercana
    std::pair<int,int> closestPill = allPills[0];
    float minDist = euclid2(pacmanCoords, allPills[0]);
    for (auto& pill : allPills) {
        float d = euclid2(pacmanCoords, pill);
        if (d < minDist) { minDist = d; closestPill = pill; }
    }

    Move desired = getClosestMove(game, closestPill);

    // busca alternativa
    if (desired == opposite) {
        std::vector<Move> moves = game.getMaze().getPossibleMoves(pacmanNode);
        for (Move m : moves) {
            if (m != opposite && game.getMaze().getNeighbour(pacmanNode, m) >= 0) {
                return m;
            }
        }
    }
    return desired;
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
	Move opposite = getOppositeDirection(currentDir);
	
	std::vector<std::pair<int,int>> ghostPositions;
	std::vector<bool> ghostsEdible;
	for(int i=0;i<4;i++){
		ghostPositions.push_back(game.getMaze().getNodePos(game.getGhostsPos(i)));
		ghostsEdible.push_back(game.isGhostEdible(i));
	}
	
	
	auto powerPillPositions=game.getMaze().getPowerPillPositions();	

	float fear=0.0f;
	Move escapeMove = PASS;
	float hunger=0.0f;
	Move eatGhostMove = PASS;
	std::vector<std::pair<int,int>> threats;
	//arrancar de fantasmas cercanos que me pueden comer 
	for(int i=0;i<4;i++){
		if((!ghostsEdible[i])){
			//cambio de formula para que el miedo sea menor
			float d = getDistanceToGhost(game,i);
			float tempFear=1.0f-1.0f/(1.0f+pow(2.718f * 0.45f,-d+20.0f));//logistica
			if(tempFear>0.15f){
				threats.push_back(ghostPositions[i]);
				if(tempFear>fear) fear = tempFear;
 			}
		}
		
	}
	//perseguir fantasmas azules
	for(int i=0;i<4;i++){
		if(ghostsEdible[i]){
			//cambio de formula para que el hambre sea mayor
			float d = getDistanceToGhost(game,i);
			float tempHunger= 1.5f * (pow(100.0f-d,2)/pow(100.0,2));//cuadratica
			if(tempHunger>hunger){
				hunger=tempHunger;
				eatGhostMove=getClosestMove(game,ghostPositions[i]);;
			}
		}
	}

	Move finalMove = PASS;

	if(fear > 0.3f && !threats.empty()){
		finalMove = getEscapeMoveFromAll(game,threats);
	}else if( hunger > 0.1f){
		if( eatGhostMove == opposite){
			std::vector<Move> moves = game.getMaze().getPossibleMoves(pacmanNode);
			bool changed = false;
			for ( Move m:moves){
				if(m != opposite && game.getMaze().getNeighbour(pacmanNode,m)>=0.){
					finalMove = m;
					changed = true;
					break;
				}
			}
			if(!changed) finalMove = eatGhostMove;
		}else{
			finalMove = eatGhostMove;
		}
	}else{
		finalMove = getExploreMove(game);
	}
	if(game.getMaze().getNeighbour(pacmanNode,finalMove)<0){
		std::vector<Move> possibleMoves= game.getMaze().getPossibleMoves(pacmanNode);
		if(!possibleMoves.empty()) finalMove = possibleMoves[0];
	}

    return finalMove;
}
