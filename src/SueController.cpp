#include "SueController.h"
#include <climits>

SueController::SueController(std::shared_ptr<Character> character):
	Controller(character){
}

SueController::~SueController() {

}

int getSqrDistance(std::pair<int,int> p1, std::pair<int,int> p2){

	return (p1.first - p2.first) * (p1.first - p2.first) + 
           (p1.second - p2.second) * (p1.second - p2.second);
}

Move
SueController::getMove(const GameState& game){

	int sueNode = character->getPos();
	auto sueCoords = game.getMaze().getNodePos(sueNode);

	if(game.isGhostEdible(3) > 0)
	{
		int maxDist =-1;
		Move escapeMove = character->getDirection();

		//obtener la posicion de pacman
		int pacmanNode = game.getPacmanPos();
		auto pacmanCoords = game.getMaze().getNodePos(pacmanNode);


		std::vector<Move> possibleMoves = game.getMaze().getPossibleMoves(sueNode);
		
		for (Move m : possibleMoves){

			int vecino = game.getMaze().getNeighbour(sueNode, m);
			if (vecino < 0) continue;

			auto vecinoCoords = game.getMaze().getNodePos(vecino);
            int dist = getSqrDistance(vecinoCoords, pacmanCoords);
            if (dist > maxDist) {
                maxDist = dist;
                escapeMove = m;

				return escapeMove;
            }
        }   
	}

	auto powerPillPositions = game.getMaze().getPowerPillPositions();

	int pacmanNode = game.getPacmanPos();
	auto pacmanCoords = game.getMaze().getNodePos(pacmanNode);
	
	if (powerPillPositions.empty())
	{
		int minDist = INT_MAX;
		Move attackMove = character->getDirection();
		std::vector<Move> possibleMoves = game.getMaze().getPossibleMoves(sueNode);

		for (Move m : possibleMoves) {
            int vecino = game.getMaze().getNeighbour(sueNode, m);
            if (vecino < 0) continue;
            
            auto vecinoCoords = game.getMaze().getNodePos(vecino);
            int dist = getSqrDistance(vecinoCoords, pacmanCoords);
            if (dist < minDist) {
                minDist = dist;
                attackMove = m;
            }
        }
        return attackMove;
	}
	// buscar la pastilla mas cercana a sue
	std::pair<int, int> targetPill = powerPillPositions[0];
    int minPillDist = INT_MAX;

    for (const auto& pillPos : powerPillPositions) {
        int dist = getSqrDistance(sueCoords, pillPos);
        if (dist < minPillDist) {
            minPillDist = dist;
            targetPill = pillPos;
        }
    }

    // el objetivo final de Sue será la píldora de poder para bloquear el paso de Pac-Man.
    std::pair<int, int> finalTarget = targetPill;

    int minDist = INT_MAX;
    Move bestMove = character->getDirection();
    std::vector<Move> possibleMoves = game.getMaze().getPossibleMoves(sueNode);

    // direccion opuesta para evitar la oscilacion
    Move currentDir = character->getDirection();
    Move oppositeDir = PASS;
    if (currentDir == UP) oppositeDir = DOWN;
    if (currentDir == DOWN) oppositeDir = UP;
    if (currentDir == LEFT) oppositeDir = RIGHT;
    if (currentDir == RIGHT) oppositeDir = LEFT;

    for (Move m : possibleMoves) {
        // no se dara la vuelta en intersecciones o pasillos si hay otras opciones
        if (possibleMoves.size() > 1 && m == oppositeDir) continue;

        int vecino = game.getMaze().getNeighbour(sueNode, m);
        if (vecino < 0) continue;
        
        auto vecinoCoords = game.getMaze().getNodePos(vecino);
        int dist = getSqrDistance(vecinoCoords, finalTarget);
        
        if (dist < minDist) {
            minDist = dist;
            bestMove = m;
        }
    }

    // Si falla su movimiento o colisiona toma el primero disponible
    if (game.getMaze().getNeighbour(sueNode, bestMove) < 0 && !possibleMoves.empty()) {
        bestMove = possibleMoves[0];
    }

    return bestMove;
}