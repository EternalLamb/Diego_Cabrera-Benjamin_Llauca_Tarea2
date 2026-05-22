
#include "Maze.h"
#include "Game.h"
#include <iostream>


/*
 * If 'main' is defined we clear that definition
 * to get our default 'main' function back.
 */
#ifdef main
# undef main
#endif /* main */
bool quick=false;
bool nogui=false;

int main(int argc, char *argv[]) {

	if(argc>1 && std::string(argv[1])==std::string("quick")){
		quick=true;
	}
	if(argc>1 && std::string(argv[1])==std::string("nogui")){
		nogui=true;
		quick=true;
	}
	Game g;
	g.run();

	return 0;
}
