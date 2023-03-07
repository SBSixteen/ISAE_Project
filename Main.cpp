#pragma once
#define SDL_MAIN_HANDLED
#include "Main.h"
#include "SDL_ttf.h"
#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_image.h>
#include "wtypes.h"
#include <iostream>
#include "Button.h"
#include "Mouse.h"
#include <string>
#include "GameModes.h"

using namespace std;

SDL_Renderer* R;

Button* button[6];
Mouse* MenuMouse;

void menuUpdate() {
	for (Button* n : button) {
		n->checkSel(MenuMouse);
		n->Draw();
	}

	MenuMouse->Draw();
}

string time_string(int s, int m, int hr) {
	string A = "";

	if (hr == 0) {
		A.append("00:");
	}
	if (hr > 0 && hr < 10) {
		A.append("0");
		A.append(to_string(hr));
		A.append(":");
	}
	if (hr >= 10) {
		A.append(to_string(hr));
		A.append(":");
	}	if (m == 0) {
		A.append("00:");
	}
	if (m > 0 && m < 10) {
		A.append("0");
		A.append(to_string(m));
		A.append(":");
	}
	if (m >= 10) {
		A.append(to_string(m));
		A.append(":");
	}	if (s == 0) {
		A.append("00");
	}
	if (s > 0 && s < 10) {
		A.append("0");
		A.append(to_string(s));
	}
	if (s >= 10) {
		A.append(to_string(s));
	}

	return A;
}

int main() {
	TTF_Init();
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);

	const int WIDTH = desktop.right, HEIGHT = desktop.bottom, //Desktop Dimensions

		FPS = 144, delay = 1000 / FPS; //Pure Frametime,

	//1 fps = 1tick

	Uint32 frame_start;
	int frame_time;

	SDL_Surface* WindowSurface;

	srand(time(0));

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "SDL Failed to initialize. Error Code : " << SDL_GetError() << endl;
	}

	SDL_Window* window = SDL_CreateWindow("ReHASH : Proving Grounds | Version 1.0", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
	WindowSurface = SDL_GetWindowSurface(window); //Creates Window Panel

	if (window == NULL) {
		std::cout << "Could not initialize SDL Window. Error Code : " << SDL_GetError() << std::endl;
		return EXIT_FAILURE;
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 8192) < 0) //2 = Stereo
	{
		std::cout << "SDL_mixer could not initialize! SDL_mixer Error: %s\n" << Mix_GetError() << std::endl;
	}

	R = SDL_CreateRenderer(window, -1, 0); //Renders Textures

	if (R) {
		std::cout << "Renderer Initialized" << std::endl;
	}

	SDL_SetWindowFullscreen(window, true);

	bool fullscreen = false;
	bool isRunning = true;

	button[0] = new Button(64, HEIGHT - 50 * 4 - 30 * 4 - 200, 256, 50, R, "Assets/Active/Encrypt_Active.png", "Assets/Active/Encrypt.png"); //Encrypt Button
	button[1] = new Button(64, HEIGHT - 50 * 3 - 30 * 3 - 200, 256, 50, R, "Assets/Active/Decrypt_Active.png", "Assets/Active/Decrypt.png"); //Decrypt Button
	button[2] = new Button(64, HEIGHT - 50 * 2 - 30 * 2 - 200, 272, 50, R, "Assets/Active/Survival_Active.png", "Assets/Active/Survival.png"); //Survival Button
	button[3] = new Button(64, HEIGHT - 50 - 36, 128, 50, R, "Assets/Active/Exit_Active.png", "Assets/Active/Exit_Inactive.png"); //Exit Button
	button[4] = new Button(WIDTH - 256, HEIGHT - 50 - 36, 256, 50, R, "Assets/Active/Credits_Active.png", "Assets/Active/Credits.png"); //Credits Button
	button[5] = new Button(56, HEIGHT - 50 - 36 - 750, 1000, 290, R, "Assets/Active/ReHash_Active.png", "Assets/Active/ReHash.png"); //Logo

	button[4]->toggleActive();

	MenuMouse = new Mouse(R);
	SDL_ShowCursor(SDL_DISABLE);

	bool menu = true; //Inside Menu Screen
	bool pause = false; //Menu Music Control
	bool rehash = false;

	//Misc Texture
	SDL_Texture* BG = Texture_Handler::Load("Assets/Block/White.png", R);
	SDL_Texture* Info_Encrypt = Texture_Handler::Load("Assets/Active/Info_Encrypt.png", R);
	SDL_Texture* Info_Decrypt = Texture_Handler::Load("Assets/Active/Info_Decrypt.png", R);

	//Music
	Mix_Music* Gameplay = Mix_LoadMUS("Sounds/ID3.mp3");
	Mix_Music* G[3];// Mix_LoadMUS("Sounds/ID3.mp3");

	G[0] = Mix_LoadMUS("Sounds/ID4.mp3");
	G[1] = Mix_LoadMUS("Sounds/ID2.mp3");
	G[2] = Mix_LoadMUS("Sounds/ID1.mp3");

	Mix_PlayMusic(Gameplay, -1);


	SDL_Rect Info;
	Info.x = 1395;
	Info.y = 375;
	Info.w = 494;
	Info.h = 592;

	SDL_Event e;

	string Default_BG = "Assets/Menu/Network - 12716_";
	string BG_Frame_Path = "";

	int bg_count = 0;
	int frames = 0;

	TTF_Font* TEXT = TTF_OpenFont("DAGGERSQUARE.otf", 128);
	TTF_Font* TEXT_S = TTF_OpenFont("DAGGERSQUARE.otf", 64);
	TTF_Font* TEXT_XS = TTF_OpenFont("DAGGERSQUARE.otf", 32);

	while (menu) {
		frame_start = SDL_GetTicks();

		if (bg_count < 10) {
			BG_Frame_Path = Default_BG.append("00");
			BG_Frame_Path = BG_Frame_Path.append(to_string(bg_count));
			BG_Frame_Path = BG_Frame_Path.append(".jpg");
		}
		else if (bg_count > 9 && bg_count < 99) {
			BG_Frame_Path = Default_BG.append("0");
			BG_Frame_Path = BG_Frame_Path.append(to_string(bg_count));
			BG_Frame_Path = BG_Frame_Path.append(".jpg");
		}
		else {
			BG_Frame_Path = Default_BG.append(to_string(bg_count));
			BG_Frame_Path = BG_Frame_Path.append(".jpg");
		}

		SDL_RenderClear(R);
		BG = Texture_Handler::Load(BG_Frame_Path.c_str(), R);
		SDL_RenderCopy(R, BG, NULL, NULL);

		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				break;
			case SDL_MOUSEBUTTONUP:
				if (e.button.button == SDL_BUTTON_LEFT) {
					//EXIT
					if (button[3]->sel) {
						menu = false;
					}

					//Credits
					if (button[4]->sel) {
						menu = false;
						Mix_HaltMusic();
					}

					//Logo
					if (button[5]->sel) {

						Mix_HaltMusic();

						int m = rand() % 3;

						Mix_PlayMusic(G[m], 0);

						SDL_Rect* name = new SDL_Rect();
						SDL_Rect* key = new SDL_Rect();
						SDL_Rect* tm = new SDL_Rect();
						SDL_Rect* ui = new SDL_Rect();


						SDL_Surface* TIME;
						SDL_Surface* ANSWER;
						SDL_Surface* KEY;
						SDL_Surface* CIPHERTEXT;

						SDL_Texture* DP_CT;
						SDL_Texture* DP_KY;
						SDL_Texture* DP_TM;
						SDL_Texture* DP_UI;

						bool playstate = true;
						int seconds = 0, minutes = 0, hours = 0; int frames = 0;  int score = 0;

						int unsigned r, g, b;
						SDL_Color textColor;

						while (playstate)
						{



							r = rand() % 256;
							g = rand() % 256;
							b = rand() % 256;

							textColor.r = r;
							textColor.g = g;
							textColor.b = b;

							string res = GameModes::Caesar();
							rehash = true;
							int Key = rand() % 26;

							CIPHERTEXT = TTF_RenderText_Solid(TEXT, res.c_str(), textColor); //Surface = Canvas
							KEY = TTF_RenderText_Solid(TEXT, to_string(Key).c_str(), textColor); //Surface = Canvas


							DP_CT = SDL_CreateTextureFromSurface(R, CIPHERTEXT);
							DP_KY = SDL_CreateTextureFromSurface(R, KEY);


							SDL_QueryTexture(DP_CT, NULL, NULL, &name->w, &name->h);
							SDL_QueryTexture(DP_KY, NULL, NULL, &key->w, &key->h);

							name->x = WIDTH / 2 - name->w / 2; name->y = HEIGHT / 3 - name->h / 2;
							key->x = WIDTH / 2 - key->w / 2; key->y = HEIGHT / 4 - key->h / 2;

							string timekeep = "";

							string x = GameModes::Caesar_E(Key, res);

							SDL_StartTextInput();

							bool renderText = false;
							string inputText = "";
							bool done = false;


							while (rehash) {

								if (Mix_Playing == 0) {
									m = rand() % 3;
									Mix_PlayMusic(G[m], 0);
								}

								frame_start = SDL_GetTicks();

								while (SDL_PollEvent(&e)) {
									if (e.type == SDL_TEXTINPUT)
									{
										//Accept Keyboard Input
										if (!(SDL_GetModState() & KMOD_CTRL && (e.text.text[0] == 'c' || e.text.text[0] == 'C' || e.text.text[0] == 'v' || e.text.text[0] == 'V')))
										{
											//Append character
											inputText += e.text.text;
											renderText = true;
										}
									}
									else if (e.type == SDL_KEYDOWN)
									{
										//Handle backspace
										if (e.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0)
										{
											//Pop off character
											inputText.pop_back();
											renderText = true;
										}
										//Submit Name
										if (e.key.keysym.sym == SDLK_RETURN && inputText.length() > 0)
										{
											for (int i = 0; i < inputText.length(); i++) {
												inputText[i] = inputText[i] & ~' ';
											}

											cout << inputText << endl;
											cout << x << endl;

											if (x == inputText) {
												cout << "Correct" << endl;
												//SDL_SetRenderDrawColor(R, 0, 255, 0, 255);
											}
											else {
												cout << "Try Again" << endl;
												//SDL_SetRenderDrawColor(R, 255, 0, 0, 255);
											}

											rehash = false;

										}

										if (e.key.keysym.sym == SDLK_F11)
										{
											SDL_MinimizeWindow(window);
										}


										if (e.key.keysym.sym == SDLK_ESCAPE)
										{
											rehash = false;
											playstate = false;
										}
									}
								}

								timekeep = time_string(seconds, minutes, hours);
								TIME = TTF_RenderText_Solid(TEXT_S, timekeep.c_str(), { 255,255,255 }); //Surface = Canvas
								DP_TM = SDL_CreateTextureFromSurface(R, TIME);
								SDL_QueryTexture(DP_TM, NULL, NULL, &tm->w, &tm->h);
								tm->x = WIDTH / 2 - tm->w / 2; tm->y = 15;

								SDL_RenderClear(R);

								if (renderText) {
									ANSWER = TTF_RenderText_Solid(TEXT, inputText.c_str(), { 255,255,255 }); //Surface = Canvas
									DP_UI = SDL_CreateTextureFromSurface(R, ANSWER);
									SDL_QueryTexture(DP_UI, NULL, NULL, &ui->w, &ui->h);
									ui->x = WIDTH / 2 - ui->w / 2; ui->y = HEIGHT / 2;
									SDL_RenderCopy(R, DP_UI, NULL, ui);
									SDL_FreeSurface(ANSWER);
									SDL_DestroyTexture(DP_UI);
								}

								SDL_RenderCopy(R, DP_CT, NULL, name);
								SDL_RenderCopy(R, DP_KY, NULL, key);
								SDL_RenderCopy(R, DP_TM, NULL, tm);

								SDL_RenderPresent(R);

								if (frames % 144 == 0) {
									seconds++;
									if ((seconds) % 60 == 0) {
										minutes++;
										seconds = 0;
										if (minutes % 60 == 0) {
											hours++;
											minutes = 0;
										}
									}
								}

								if (frames > 1440) {
									frames = 0;
								}

								SDL_FreeSurface(TIME);
								SDL_DestroyTexture(DP_TM);

								frame_time = SDL_GetTicks() - frame_start;

								SDL_Delay(delay - frame_time);

								frames++;
							}

							SDL_StopTextInput();
							SDL_DestroyTexture(DP_CT);
							SDL_DestroyTexture(DP_KY);
							SDL_FreeSurface(CIPHERTEXT);
							SDL_FreeSurface(KEY);
						}
					}
				}
			}
		}

		SDL_GetMouseState(&(MenuMouse->cursor.x), &(MenuMouse->cursor.y));

		menuUpdate();
		if (button[0]->sel) {
			SDL_RenderCopy(R, Info_Encrypt, NULL, &Info);
		}
		if (button[1]->sel) {
			SDL_RenderCopy(R, Info_Decrypt, NULL, &Info);
		}

		SDL_RenderPresent(R);

		frame_time = SDL_GetTicks() - frame_start;

		if (delay > frame_time) {
		}

		frames++;

		if (frames % 4 == 0) {
			bg_count++;
			if (bg_count == 201) {
				bg_count = 0;
			}
		}

		//Cleanup and Reset
		Default_BG = "Assets/Menu/Network - 12716_";
		BG_Frame_Path = "";
		SDL_DestroyTexture(BG);
	}
}