/*
 * GameView.h
 *
 *  Created on: Apr 17, 2018
 *      Author: VIDEOJUEGOS UTALCA
 */

#ifndef GAMEVIEW_H_
#define GAMEVIEW_H_
#include "GameState.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <memory>

class GameView {
	SDL_Window *win;
	SDL_Renderer *ren;
	std::vector<std::shared_ptr<SDL_Texture>> mapTex;
	std::shared_ptr<SDL_Texture> pacTex;
	std::array<std::shared_ptr<SDL_Texture>,4> ghostTex;
	std::shared_ptr<SDL_Texture> edibleGhostTex;
	std::shared_ptr<SDL_Texture> load(const std::string &filename) const;
public:
	GameView(std::vector<std::string> mazeImages);
	virtual ~GameView();
	void draw(int mapNumber,const GameState &state)const;
};

#endif /* GAMEVIEW_H_ */
