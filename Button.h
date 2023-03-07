#pragma once
#include <SDL.h>
#include <SDL_Mixer.h>
#include "Texture_Handler.h"
#include "Mouse.h"
#include <string>

using namespace std;

class Button
{
public:
	SDL_Texture* Texture; 
	SDL_Texture* Texture_Inactive;
	SDL_Rect sR;
	bool sel, sound, toggle;
	Mix_Chunk* Hover;
	SDL_Renderer* ren;
	Button(SDL_Renderer* R);
	Button(int x, int y, int w, int h, SDL_Renderer* R, string filePath, string filePath_I);
	Button(int x, int y, int w, int h, SDL_Renderer* R, string filePath);
	void Draw();
	void checkSel(Mouse* mouse); //Check if mouse is on top of it
	void toggleActive();
	~Button();
};

