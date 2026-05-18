#include "InkyController.h"
#include "Ghost.h"
#include <SDL2/SDL.h>
#include <cmath>
#include <cstdlib> // Randoms para que inky sea flojo

InkyController::InkyController(std::shared_ptr<Character> character):
    Controller(character){
}

InkyController::~InkyController() {
}

Move InkyController::getClosestMove(const GameState& game, std::pair<int,int> target) const {
    int minDist = 10000000;
    Move minMove = character->getDirection();
    std::vector<Move> moves = game.getMaze().getPossibleMoves(character->getPos());
    for(Move m : moves){
        int vecino = game.getMaze().getNeighbour(character->getPos(), m);
        if(vecino < 0) continue;
        auto vecinoCoords = game.getMaze().getNodePos(vecino);
        int sqDist = euclid2(vecinoCoords, target);
        if(sqDist < minDist){
            minDist = sqDist;
            minMove = m;
        }
    }
    return minMove;
}

Move InkyController::getFarthestMove(const GameState& game, std::pair<int,int> target) const {
    int maxDist = -1;
    Move maxMove = character->getDirection();
    std::vector<Move> moves = game.getMaze().getPossibleMoves(character->getPos());
    for(Move m : moves){
        int vecino = game.getMaze().getNeighbour(character->getPos(), m);
        if(vecino < 0) continue;
        auto vecinoCoords = game.getMaze().getNodePos(vecino);
        int sqDist = euclid2(vecinoCoords, target);
        if(sqDist > maxDist){
            maxDist = sqDist;
            maxMove = m;
        }
    }
    return maxMove;
}

// Función para que Inky se pasee cuando pacman no esta cerca
Move InkyController::getRandomMove(const GameState& game) const {
    std::vector<Move> moves = game.getMaze().getPossibleMoves(character->getPos());
    if(moves.empty()) return PASS;
    
    int randomIndex = std::rand() % moves.size();
    return moves[randomIndex];
}

float InkyController::getDistanceToPacman(const GameState& game) const {
    int pacmanNode = game.getPacmanPos(); 
    return std::sqrt(euclid2(
        game.getMaze().getNodePos(character->getPos()),
        game.getMaze().getNodePos(pacmanNode)));
}

Move InkyController::getMove(const GameState& game) {
    // Cerrar la ventana con SDL
    SDL_Event e;
    if( SDL_PollEvent( &e ) != 0 ) {
        if( e.type == SDL_QUIT || 
            (e.type == SDL_KEYDOWN && 
                (e.key.keysym.sym==SDLK_ESCAPE || e.key.keysym.sym==SDLK_q) )) {
            SDL_Quit();
            exit(0);
        }
    }
    
    int pacmanNode = game.getPacmanPos();
    auto pacmanCoords = game.getMaze().getNodePos(pacmanNode);
    float distance = getDistanceToPacman(game);

    auto ghostPtr = std::dynamic_pointer_cast<Ghost>(character);
    bool ImEdible = (ghostPtr != nullptr) ? ghostPtr->isEdible() : false; 

    float panic = 0.0f;
    float aggressiveness = 0.0f;
    
    Move escapeMove = PASS;
    Move eatGhostMove = PASS;
    Move lazyMove = PASS;

    if (ImEdible) {

        panic = 1.0f - 1.0f / (1.0f + std::pow(2.718f * 0.45f, -distance + 50.0f));
        escapeMove = getFarthestMove(game, pacmanCoords);
    } else {
        // --- Inky el fantasma distraido :P ---
        // Amenos que Pacman no este en su rango, lo ignorara y dará vueltas como menso.
        // Solo si esta cerca lo perseguira 
        float radioDeteccion = 40.0f; 

        if (distance <= radioDeteccion) {
            aggressiveness = std::pow((radioDeteccion - distance) / radioDeteccion, 2);
            eatGhostMove = getClosestMove(game, pacmanCoords);
        } else {
            aggressiveness = 0.0f;
            lazyMove = getRandomMove(game); 
        }
    }

    // Selector del Utility System 
    if (ImEdible) {
        return (panic > aggressiveness) ? escapeMove : eatGhostMove;
    } else {
        // Si no es comestible y el hambre es > 0, significa que Pacman está cerca
        if (aggressiveness > 0.0f) {
            return eatGhostMove;
        } else {
            // Si el hambre es 0, Inky vaga sin rumbo
            return lazyMove;
        }
    }
}