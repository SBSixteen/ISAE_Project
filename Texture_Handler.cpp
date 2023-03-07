#include "Texture_Handler.h"

SDL_Texture* Texture_Handler::Load(const char* path, SDL_Renderer* R) { //TEXTURE_LOADIMG

	SDL_Surface* temp = IMG_Load(path);
	SDL_Texture* T = SDL_CreateTextureFromSurface(R, temp);

	SDL_FreeSurface(temp);

	return T;
}

void Texture_Handler::Draw(SDL_Texture* T, SDL_Rect sR, SDL_Rect dR, SDL_Renderer* R) {

	//SDL_RenderCopy();

}