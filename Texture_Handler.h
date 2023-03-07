#pragma once
#include <SDL.h>
#include <SDL_image.h>

class Texture_Handler
{
public:
	static SDL_Texture* Load(const char* path, SDL_Renderer* R);
	static void Draw(SDL_Texture* T, SDL_Rect sR, SDL_Rect dR, SDL_Renderer* R);
};
