#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "GameVariables.h"
using std::cout;
using std::endl;

struct TextureManager {
	static SDL_Texture* LoadTexture(const char* filename){
		SDL_Surface* surface = IMG_Load(filename);
		if (surface == NULL) {
			cout << "Surface did not load!" << endl;
			cout << IMG_GetError() << endl;
		}
		SDL_Texture* texture = SDL_CreateTextureFromSurface(RENDERER, surface);
		if (texture == nullptr) {
			cout << "Texture did not load!" << endl;
		}
		if (RENDERER == nullptr) {
			cout << "That's because the renderer hasn't been made yet!" << endl;
		}
		SDL_FreeSurface(surface);

		return texture;
	}

	static void Draw(SDL_Texture* texture, const SDL_Rect& src, 
	  const SDL_Rect& dest, double angle = 0.0, 
      SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE){
		SDL_RenderCopyEx(RENDERER, texture, &src, &dest, angle, center, flip);
		}

};



#endif
