#pragma once
#include "Texture_Handler.h"

class Mouse
{

public:
	Mouse(SDL_Renderer* R);
	Mouse(SDL_Renderer* R, int i);
	~Mouse();
	void Draw();
	SDL_Texture* texture;
	SDL_Rect cursor, tip; //Cursor loads the png, 
	bool sel;
	SDL_Renderer* ren;

};

