/*
 * GameView.cpp
 *
 *  Created on: Apr 17, 2018
 *      Author: VIDEOJUEGOS UTALCA
 */

#include "GameView.h"
#include <iostream>

extern bool quick;
extern bool nogui;

std::shared_ptr<SDL_Texture> GameView::load(const std::string &filename) const{
	if(nogui)
		return std::shared_ptr<SDL_Texture>(nullptr);
	SDL_Surface *bmp = IMG_Load(filename.c_str());
	if (bmp == nullptr){
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		std::cerr << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		throw;
	}

	std::shared_ptr<SDL_Texture> tex = std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(ren, bmp),SDL_DestroyTexture);
	SDL_FreeSurface(bmp);
	if (tex == nullptr){
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		throw;
	}
	return tex;
}
GameView::GameView(std::vector<std::string> mazeImages) {
	if(nogui)
		return;
	if (SDL_Init(SDL_INIT_VIDEO) != 0){
			std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
			throw;
		}
	win = SDL_CreateWindow("Hello World!", 100, 100, 435, 488, SDL_WINDOW_SHOWN);
	if (win == nullptr){
		std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		throw;
	}

	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr){
		SDL_DestroyWindow(win);
		std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		throw;
	}



	for(auto &file:mazeImages){
		mapTex.push_back(load(file));
	}
	pacTex=load("images/mspacman-right-normal.png");
	ghostTex[0]=load("images/blinky-right-1.png");
	ghostTex[1]=load("images/inky-right-1.png");
	ghostTex[2]=load("images/pinky-right-1.png");
	ghostTex[3]=load("images/sue-right-1.png");
	edibleGhostTex=load("images/edible-ghost-1.png");
}

GameView::~GameView() {
	if(nogui)
		return;
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
}

void GameView::draw(int mapNumber, const GameState& state) const {

	if(nogui)
		return;
	const int scale=4;
	//First clear the renderer
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
	SDL_RenderClear(ren);
	SDL_RenderCopy(ren, mapTex[mapNumber].get(), NULL, NULL);
	for(const auto &pos:state.maze.getPillPositions()){
		SDL_RenderDrawPoint(ren,pos.first*scale,pos.second*scale);
		filledEllipseRGBA(ren,pos.first*scale,pos.second*scale,2,1,255,255,0,255);
		//std::cerr<<pos.first<<" "<<pos.second<<std::endl;
	}
	for(const auto &pos:state.maze.getPowerPillPositions()){
			SDL_RenderDrawPoint(ren,pos.first*scale,pos.second*scale);
			filledEllipseRGBA(ren,pos.first*scale,pos.second*scale,4,4,255,255,0,255);
			//std::cerr<<pos.first<<" "<<pos.second<<std::endl;
		}

	auto pacPos=state.maze.getNodePos(state.pacman->currentNodeIndex);
	SDL_Rect dest;
	dest.w=dest.h=28;
	dest.x=pacPos.first*scale-dest.w/2;
	dest.y=pacPos.second*scale-dest.h/2;
	//Draw the texture
	SDL_RenderCopy(ren, pacTex.get(), NULL, &dest);

	for(int i=0;i<4;i++){
		auto ghostPos=state.maze.getNodePos(state.ghosts[i]->currentNodeIndex);
		SDL_Rect dest;
		dest.w=dest.h=28;
		dest.x=ghostPos.first*scale-dest.w/2;
		dest.y=ghostPos.second*scale-dest.h/2;
		//Draw the texture
		if(!state.ghosts[i]->isEdible()){
			SDL_RenderCopy(ren, ghostTex[i].get(), NULL, &dest);
		}else{
			SDL_RenderCopy(ren, edibleGhostTex.get(), NULL, &dest);
		}
	}

	//Update the screen
	SDL_RenderPresent(ren);
	//Take a quick break after all that hard work
	if(!quick)SDL_Delay(20);

}
