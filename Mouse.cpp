#include "Mouse.h"

Mouse::Mouse(SDL_Renderer* R) {
	ren = R;
	texture = Texture_Handler::Load("Assets/Block/Cursor.png", ren);
	cursor = { 0,0,25,25 };
	tip = { 0,0,1,1 };
	sel = false;
}

Mouse::Mouse(SDL_Renderer* R, int i) {
	ren = R;
	texture = Texture_Handler::Load("Assets/Crosshair.png", ren);
	cursor = { 0,0,25,25 };
	tip = { 12,12,1,1 };
}

Mouse::~Mouse()
{
}

void Mouse::Draw() {
	tip.x = cursor.x;
	tip.y = cursor.y;
	SDL_RenderCopy(ren, texture, NULL, &cursor);
}