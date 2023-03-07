#include <SDL_mixer.h>
#include <iostream>
#include <string>
#include "Button.h"

using namespace std;

Button::Button(SDL_Renderer* R) :sR{ 0,0,128,32 } {
	static SDL_Texture* t = Texture_Handler::Load("/Assets/Menu Buttons.png", R);
	Texture = t;
	sel = false;
	ren = R;
	Hover = Mix_LoadWAV("Sounds/Mouse_Hover_Text.wav");
	sound = false;
	toggle = false;
};

Button::Button(int x, int y, int w, int h, SDL_Renderer* R, string filePath, string filePath_I) {
	sR.x = x; sR.y = y; sR.w = w; sR.h = h;

	Texture = Texture_Handler::Load(filePath.c_str(), R);
	Texture_Inactive = Texture_Handler::Load(filePath_I.c_str(), R);

	if (Texture == NULL) {
		cout << "Image file not found" << endl;
	}
	if (Texture_Inactive == NULL) {
		cout << "Image file not found" << endl;
	}

	sel = false;
	ren = R;
	Hover = Mix_LoadWAV("Sounds/Mouse_Hover_Text.wav");
	sound = false;
	toggle = true;
}

void Button::toggleActive() {
	if (!toggle) {
		toggle = true;
	}
	else {
		toggle = false;
	}
}

Button::~Button() {
}

void Button::checkSel(Mouse* mouse) {
	if (SDL_HasIntersection(&(mouse->tip), &sR)) {
		sel = true;

		if (!sound) {
			Mix_PlayChannel(-1, Hover, 0);
			sound = true;
			if (toggle) {
				sR.x = sR.x + 64;
			}
			else {
				sR.x = sR.x - 64;
			}
		}
	}
	else {
		if (sound) {
			sound = true;
			if (toggle) {
				sR.x = sR.x - 64;
			}
			else {
				sR.x = sR.x + 64;
			}
		}
		sel = false;
		sound = false;
	}
}

void Button::Draw() {
	if (sel) {
		SDL_RenderCopy(ren, Texture, NULL, &sR);
	}
	else {
		SDL_RenderCopy(ren, Texture_Inactive, NULL, &sR);
	}
}