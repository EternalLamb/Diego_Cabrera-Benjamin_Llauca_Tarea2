#include "InkyController.h"
#include "Ghost.h"
#include <SDL2/SDL.h>
#include <cmath>
#include <cstdlib>

Inky_Info* Inky_Info::info = nullptr;

InkyController::InkyController(std::shared_ptr<Character> character)
    : Controller(character), root(std::make_shared<Selector>())
{
    auto filterFright = std::make_shared<Filter>();
    filterFright->addCondition(std::make_shared<Powerpill_Inky>());
    filterFright->addAction(std::make_shared<Frightened_Inky>());
    root->addChild(filterFright);

    auto filterWander = std::make_shared<Filter>();
    filterWander->addCondition(std::make_shared<OutOfRange_Inky>());
    filterWander->addAction(std::make_shared<Wander_Inky>());
    root->addChild(filterWander);

    root->addChild(std::make_shared<Chase_Inky>());
}

InkyController::~InkyController() {}

Move InkyController::getMove(const GameState& gs) {
    SDL_Event e;
    if (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT ||
            (e.type == SDL_KEYDOWN &&
             (e.key.keysym.sym == SDLK_ESCAPE || e.key.keysym.sym == SDLK_q))) {
            SDL_Quit();
            exit(0);
        }
    }

    Inky_Info::getInfo()->in_character  = character;
    Inky_Info::getInfo()->in_gamestate  = &gs;
    root->tick();
    return Inky_Info::getInfo()->out_move;
}
static float inkyDistToPacman(const GameState& gs, std::shared_ptr<Character> ch) {
    return std::sqrt(static_cast<float>(euclid2(
        gs.getMaze().getNodePos(ch->getPos()),
        gs.getMaze().getNodePos(gs.getPacmanPos()))));
}

Status Powerpill_Inky::update() {
    auto ch    = Inky_Info::getInfo()->in_character;
    auto ghost = dynamic_cast<Ghost*>(ch.get());
    return (ghost != nullptr && ghost->isEdible()) ? BH_SUCCESS : BH_FAILURE;
}

Status OutOfRange_Inky::update() {
    auto ch = Inky_Info::getInfo()->in_character;
    auto gs = Inky_Info::getInfo()->in_gamestate;
    float d = inkyDistToPacman(*gs, ch);
    return (d > RADIO) ? BH_SUCCESS : BH_FAILURE;
}


Status Frightened_Inky::update() {
    auto ch = Inky_Info::getInfo()->in_character;
    auto gs = Inky_Info::getInfo()->in_gamestate;

    auto pacmanCoords = gs->getMaze().getNodePos(gs->getPacmanPos());

    int  maxDist = -1;
    Move maxMove = ch->getDirection();

    std::vector<Move> moves = gs->getMaze().getPossibleMoves(ch->getPos());
    for (Move m : moves) {
        int vecino = gs->getMaze().getNeighbour(ch->getPos(), m);
        if (vecino < 0) continue;
        int d = euclid2(gs->getMaze().getNodePos(vecino), pacmanCoords);
        if (d > maxDist) { maxDist = d; maxMove = m; }
    }

    Inky_Info::getInfo()->out_move = maxMove;
    return BH_SUCCESS;
}

Status Wander_Inky::update() {
    auto ch = Inky_Info::getInfo()->in_character;
    auto gs = Inky_Info::getInfo()->in_gamestate;

    std::vector<Move> moves = gs->getMaze().getPossibleMoves(ch->getPos());
    if (moves.empty()) {
        Inky_Info::getInfo()->out_move = PASS;
        return BH_SUCCESS;
    }
    Inky_Info::getInfo()->out_move = moves[std::rand() % moves.size()];
    return BH_SUCCESS;
}

Status Chase_Inky::update() {
    auto ch = Inky_Info::getInfo()->in_character;
    auto gs = Inky_Info::getInfo()->in_gamestate;

    auto pacmanCoords = gs->getMaze().getNodePos(gs->getPacmanPos());

    int  minDist = 10000000;
    Move minMove = ch->getDirection();

    std::vector<Move> moves;
    if (ch->getDirection() == PASS)
        moves = gs->getMaze().getPossibleMoves(ch->getPos());
    else
        moves = gs->getMaze().getGhostLegalMoves(ch->getPos(), ch->getDirection());

    for (Move m : moves) {
        if (m == PASS) break;
        int vecino = gs->getMaze().getNeighbour(ch->getPos(), m);
        if (vecino < 0) continue;
        int d = euclid2(gs->getMaze().getNodePos(vecino), pacmanCoords);
        if (d < minDist) { minDist = d; minMove = m; }
    }

    Inky_Info::getInfo()->out_move = minMove;
    return BH_SUCCESS;
}