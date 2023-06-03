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
#include <vector>

using namespace std;

SDL_Renderer* R;

Button* button[6];

Mouse* MenuMouse;

const int distract = 64; // Amount of numbers to render.

int symposium_limit = 5;

bool toggle_bg = true;
bool toggle_dt = false;
bool toggle_help = false;

int menu_nav = -1;

void initButton(int HEIGHT, int WIDTH) {
	button[0] = new Button(64, HEIGHT - 50 * 4 - 30 * 4 - 200, 256, 50, R, "Assets/Active/Encrypt_Active.png", "Assets/Active/Encrypt.png"); //Encrypt Button
	button[1] = new Button(64, HEIGHT - 50 * 3 - 30 * 3 - 200, 256, 50, R, "Assets/Active/Decrypt_Active.png", "Assets/Active/Decrypt.png"); //Decrypt Button
	button[2] = new Button(64, HEIGHT - 50 * 2 - 30 * 2 - 200, 380, 50, R, "Assets/Active/Survival_Active.png", "Assets/Active/Survival.png"); //Survival Button
	button[3] = new Button(64, HEIGHT - 50 - 36, 128, 50, R, "Assets/Active/Exit_Active.png", "Assets/Active/Exit_Inactive.png"); //Exit Button
	button[4] = new Button(WIDTH - 256, HEIGHT - 50 - 36, 256, 50, R, "Assets/Active/Credits_Active.png", "Assets/Active/Credits.png"); //Credits Button
	button[5] = new Button(56, HEIGHT - 50 - 36 - 750, 1000, 290, R, "Assets/Active/ReHash_Active.png", "Assets/Active/ReHash.png"); //Logo

	button[4]->toggleActive();
}

void menuUpdate() {
	for (int i = 0; i < 6; i++) {
		button[i]->checkSel(MenuMouse);
		if (i == menu_nav) {
			button[i]->sel = true;
		}
		button[i]->Draw();
	}

	MenuMouse->Draw();
}

string time_string(int s, int m, int hr) { //Converts 3 integer to HH:MM:SS.
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

	const int

		WIDTH = desktop.right,
		HEIGHT = desktop.bottom, //Desktop Dimensions
		FPS = 144,
		delay = 1000 / FPS; //Pure Frametime,

	cout << WIDTH << " x " << HEIGHT << " | Desktop Resolution" << endl;

	//1 fps = 1tick

	Uint32 frame_start;
	int frame_time;

	SDL_Surface* WindowSurface;

	srand(time(0));

	//cout << GameModes::Vignere_E("ABC", "ABC") << endl;

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "SDL Failed to initialize. Error Code : " << SDL_GetError() << endl;
	}

	SDL_Window* window = SDL_CreateWindow("ReHASH : Proving Grounds | Version 1.0", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
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

	SDL_RenderSetLogicalSize(R, WIDTH, HEIGHT);
	SDL_SetWindowFullscreen(window, SDL_FALSE);
	SDL_SetWindowResizable(window, SDL_FALSE);

	bool fullscreen = false;
	bool isRunning = true;

	initButton(HEIGHT, WIDTH);

	//Mouse Tools
	MenuMouse = new Mouse(R);
	SDL_ShowCursor(SDL_DISABLE);

	bool menu = true; //Inside Menu Screen
	bool pause = false; //Menu Music Control
	bool rehash = false; //Inside the Text Typing Mode

	//Misc Texture
	SDL_Texture* BG = Texture_Handler::Load("Assets/Block/White.png", R);
	SDL_Texture* Info_Encrypt = Texture_Handler::Load("Assets/Active/Info_Encrypt.png", R);
	SDL_Texture* Info_Decrypt = Texture_Handler::Load("Assets/Active/Info_Decrypt.png", R);
	SDL_Texture* NONE = Texture_Handler::Load("Assets/Block/Unknown.png", R);
	SDL_Texture* CRCT = Texture_Handler::Load("Assets/Block/Correct.png", R);
	SDL_Texture* WRNG = Texture_Handler::Load("Assets/Block/Wrong.png", R);
	SDL_Texture* Help_Caeser = Texture_Handler::Load("Assets/Block/Help_Caesar.png", R);
	SDL_Texture* Help_Vignere = Texture_Handler::Load("Assets/Block/Help_Vignere.png", R);

	//Music
	Mix_Music* Gameplay = Mix_LoadMUS("Sousnds/ID3.mp3");
	Mix_Music* G[3];// Mix_LoadMUS("Sounds/ID3.mp3");
	Mix_Music* correct_effect = Mix_LoadMUS("Sounds/SUCCESS.mp3");

	//Music Array for Random Tracks through index
	G[0] = Mix_LoadMUS("Soundsv/ID4.mp3");
	G[1] = Mix_LoadMUS("Soundvs/ID2.mp3");
	G[2] = Mix_LoadMUS("Sounvds/ID1.mp3");

	Mix_PlayMusic(Gameplay, -1);

	//Recents Boxes
	vector<SDL_Rect> rcnts;
	vector<int> list;

	int recent_limit = 10;

	for (int i = 0; i < recent_limit; i++) {
		SDL_Rect G;
		G.x = (WIDTH)-48;
		G.y = 15 + (i * 48);
		G.w = 32;
		G.h = 32;
		rcnts.push_back(G);
		list.push_back(-1);
	}

	//Information Boxes
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

	//Font Initiailizations
	TTF_Font* TEXT = TTF_OpenFont("DAGGERSQUARE.otf", 128);
	TTF_Font* TEXT_S = TTF_OpenFont("DAGGERSQUARE.otf", 64);
	TTF_Font* TEXT_XS = TTF_OpenFont("DAGGERSQUARE.otf", 32);
	TTF_Font* TEXT_XXS = TTF_OpenFont("DAGGERSQUARE.otf", 16);
	TTF_Font* NUMBERS = TTF_OpenFont("Numbers.ttf", 128);

	//Version Text Display
	SDL_Rect* V = new SDL_Rect();
	SDL_Surface* Version;
	SDL_Texture* DP_VR;
	string ver = "ReHash 0.5.3 | Helping Hands Update";
	Version = TTF_RenderText_Blended(TEXT_XXS, ver.c_str(), { 255,255,255 }); //Surface = Canvas
	DP_VR = SDL_CreateTextureFromSurface(R, Version);
	SDL_QueryTexture(DP_VR, NULL, NULL, &V->w, &V->h);
	SDL_FreeSurface(Version);
	V->x = WIDTH / 2 - (V->w / 2);
	V->y = 16;

	//Helptext Display
	SDL_Rect* helptext = new SDL_Rect();
	SDL_Surface* HELPTEXT;
	SDL_Texture* DP_HT; // HT -> Helptext at the bottom of the screen
	string htxt = "F1 : Help | F9 : Toggle Experimental Effects | F10 : Switch Contrast | F11 : Minimize";
	
	//User Input Color
	SDL_Color UI_Color;
	UI_Color.r = 255;
	UI_Color.g = 255;
	UI_Color.b = 255;

	//Numbers Color
	SDL_Color NM_Color;
	NM_Color.r = 255;
	NM_Color.g = 255;
	NM_Color.b = 255;

	//Rendering Rules
	SDL_RenderSetIntegerScale(R, SDL_TRUE);

	//Playstate Initiailization
	int m = rand() % 3;
	string ty = "";

	Mix_PlayMusic(G[m], 0);

	SDL_Rect* name = new SDL_Rect(); //Rect for the randomly selected text
	SDL_Rect* key = new SDL_Rect(); //Rect for encrrypt/decrypt key
	SDL_Rect* tm = new SDL_Rect(); //Rect for Time
	SDL_Rect* ui = new SDL_Rect(); //Rect for User Input
	SDL_Rect* type = new SDL_Rect(); //Rect for
	SDL_Rect* number[distract]; //Rect for the random numbers array;


	bool n_show[distract];

	//Surfaces to generate various textures
	SDL_Surface* TIME;
	SDL_Surface* ANSWER;
	SDL_Surface* KEY;
	SDL_Surface* CIPHERTEXT;
	SDL_Surface* TYPE;
	SDL_Surface* NUM;


	//Generated Textures are stored over here
	SDL_Texture* DP_CT; //CT -> Ciphertext
	SDL_Texture* DP_KY; // KY -> Key
	SDL_Texture* DP_TM; // TM -> Time
	SDL_Texture* DP_UI; // UI -> User Input
	SDL_Texture* DP_TY; // TY -> Text in Question
	SDL_Texture* DP_NM[10]; // NM[i] -> 0-9 Numbers are stored in these using my custom font

	bool playstate = true; //Inside Play_:
	int seconds = 0, minutes = 0, hours = 0; int score = 0; //For time keeping

	int gen[distract]; //Which number to display on which box

	int unsigned r, g, b;
	SDL_Color textColor;

	//Generate Textures using NUMBERS Font in Single Color
	for (int i = 0; i < distract; i++) {
		if (i < 10) {
			NUM = TTF_RenderText_Blended(NUMBERS, to_string(i).c_str(), NM_Color); //Surface = Canvas
			DP_NM[i] = SDL_CreateTextureFromSurface(R, NUM);

			SDL_FreeSurface(NUM);
		}
		number[i] = new SDL_Rect();
		number[i]->x = (WIDTH / 8) * (i % 8) + 32;
		number[i]->y = (HEIGHT / 9) * (i / 8) + 16;
		gen[i] = rand() % 10;

		if (rand() % 2 == 0) {
			n_show[i] = false;
		}
		else {
			n_show[i] = true;
		}
	}

	cout << "ALL TEXTURES CREATED" << endl;

	bool mouse_active = true;

	while (menu) {
		frame_start = SDL_GetTicks();

		//PICK CORRECT JPG FRAME TO PLAY AS VIDEO
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
		SDL_RenderCopy(R, DP_VR, NULL, V);

		//Register User Input
		while (SDL_PollEvent(&e)) {
			;

			switch (e.type) {
			case SDL_QUIT:
				break;

			case SDL_KEYUP:

				if (e.key.keysym.sym == SDLK_DOWN) {
					//cout << "Recorded Button down" << menu_nav << endl;
					mouse_active = false;
					menu_nav++;
					if (menu_nav > 5) {
						menu_nav = 0;
					}
					if (menu_nav < 0) {
						menu_nav = 5;
					}
					//	cout << "Can use mouse? -> " << mouse_active << endl;
				}
				if (e.key.keysym.sym == SDLK_UP) {
					//cout << "Recorded Button up :" << menu_nav << endl;
					mouse_active = false;
					menu_nav--;
					if (menu_nav < 0) {
						menu_nav = 5;
					}
				}
				if (e.key.keysym.sym == SDLK_RETURN) {
					if (menu_nav == 5) {
						goto Play_; //Inspiration from portal || BLUE PORTAL ->
					}

					if (menu_nav == 3 || menu_nav == 4) {
						goto Exit_;
					}
				}

			case SDL_MOUSEBUTTONUP:

				if (e.button.button == SDL_BUTTON_LEFT && mouse_active) {
					//How to play
					if (button[2]->sel) {
					}

					//EXIT
					if (button[3]->sel) {
					Exit_:
						menu = false;
						Mix_HaltMusic();
					}

					//Credits
					if (button[4]->sel) {
						menu = false;
					}

					//Logo
					if (button[5]->sel) {
					Play_: //Inspiration from portal || ORANGE PORTAL <-
						Mix_HaltMusic();
						playstate = true;
						toggle_dt = false;
						int frames = 0;
						hours = 0; minutes = 0; seconds = 0;

						SDL_Event f;

						list.clear();

						for (int i = 0; i < recent_limit; i++) {
							list.push_back(-1);
						}

						while (playstate)
						{
							while (SDL_PollEvent(&e)) {
								switch (e.type) {
								case SDL_QUIT:
									break;
								}
							}

							rehash = true;

							r = rand() % 256;
							g = rand() % 256;
							b = rand() % 256;

							//cout << SDL_GetErrorMsg() << endl;

							textColor.r = r;
							textColor.g = g;
							textColor.b = b;

							string res = GameModes::get_Word(); //res is the thing to display

							int s_gamemode = rand() % 4;

							string Key = "";
							string x = ""; //X is the correct final answer

							//cout << s_gamemode << endl;

							if (s_gamemode == 0) {
								int k = rand() % 26;
								Key = to_string(k);
								x = GameModes::Caesar_E(k, res);
								ty = "Caesar's Encryption";
							}

							if (s_gamemode == 1) {
								//cout << "Doing Vignere" << endl;
								Key = GameModes::Vignere_K();
								//cout << "Key Made" << endl;
								x = GameModes::Vignere_E(Key, res);
								//cout << "Generated Ciphertext" << endl;
								ty = "Vignere's Encryption";
							}

							if (s_gamemode == 2) {
								int k = rand() % 26;
								Key = to_string(k);
								string temp = GameModes::Caesar_E(k, res);
								x = res;
								res = temp;
								ty = "Caesar's Decryption";
							}

							if (s_gamemode == 3) {
								Key = GameModes::Vignere_K();
								x = res;
								res = GameModes::Vignere_E(Key, res);
								ty = "Vingere's Decryption";
							}

							CIPHERTEXT = TTF_RenderText_Blended(TEXT, res.c_str(), textColor); //Surface = Canvas
							KEY = TTF_RenderText_Blended(TEXT, Key.c_str(), textColor); //Surface = Canvas
							TYPE = TTF_RenderText_Blended(TEXT_XS, ty.c_str(), textColor); //Surface = Canvas
							HELPTEXT = TTF_RenderText_Blended(TEXT_XXS, htxt.c_str(), textColor);

							DP_CT = SDL_CreateTextureFromSurface(R, CIPHERTEXT);
							DP_TY = SDL_CreateTextureFromSurface(R, TYPE);
							DP_KY = SDL_CreateTextureFromSurface(R, KEY);
							DP_HT = SDL_CreateTextureFromSurface(R, HELPTEXT);

							SDL_QueryTexture(DP_CT, NULL, NULL, &name->w, &name->h);
							SDL_QueryTexture(DP_KY, NULL, NULL, &key->w, &key->h);
							SDL_QueryTexture(DP_TY, NULL, NULL, &type->w, &type->h);
							SDL_QueryTexture(DP_HT, NULL, NULL, &helptext->w, &helptext->h);

							SDL_FreeSurface(TYPE);

							name->x = WIDTH / 2 - name->w / 2; name->y = HEIGHT / 3 - name->h / 2;
							key->x = WIDTH / 2 - key->w / 2; key->y = HEIGHT / 4 - 100;
							type->x = 32; type->y = HEIGHT - (type->h * 3) / 2;
							helptext->x = WIDTH / 2 - helptext->w / 2; helptext->y = HEIGHT - (helptext->h * 3) / 2;

							string timekeep = "";

							bool renderText = false;
							string inputText = "";

							while (rehash) {
								if (Mix_PlayingMusic() == 0) {
									m = rand() % 3;
									Mix_PlayMusic(G[m], 0);
								}

								//cout << SDL_GetError() << " | Inside Text Input " << endl;

								SDL_RenderClear(R);

								//cout << SDL_GetError() << " | After Render Clear " << endl;

								while (SDL_PollEvent(&f) != 0) {
									//	cout << SDL_GetError() << " | Polling Start " << endl;

									if (f.type == SDL_TEXTINPUT)
									{
										//Accept Keyboard Input
										if (!(SDL_GetModState() & KMOD_CTRL && (f.text.text[0] == 'c' || f.text.text[0] == 'C' || f.text.text[0] == 'v' || f.text.text[0] == 'V')))
										{
											//Append character
											inputText += f.text.text;
											renderText = true;
										}
									}
									else if (f.type == SDL_KEYDOWN)
									{
										//Handle backspace
										if (f.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0)
										{
											//Pop off character
											inputText.pop_back();
											renderText = true;
										}
										//Submit Name
										if (f.key.keysym.sym == SDLK_RETURN && inputText.length() > 0)
										{
											for (int i = 0; i < inputText.length(); i++) {
												inputText[i] = inputText[i] & ~' ';
											}

											//cout << inputText << endl;
											//cout << x << endl;
											list.pop_back();
											if (x == inputText) {
												//cout << "Correct" << endl;
												list.insert(list.begin(), 1);
												score++;
												Mix_PlayMusic(correct_effect, 0);
												if (toggle_dt) {
													toggle_dt = false;
												}
											}
											else {
												//cout << "Try Again" << endl;
												//SDL_SetRenderDrawColor(R, 255, 0, 0, 255);
												list.insert(list.begin(), 0);
												if (!toggle_dt) {
													//toggle_dt = true;
												}
											}

											rehash = false;
										}

										if (f.key.keysym.sym == SDLK_F11)
										{
											SDL_MinimizeWindow(window);
										}

										if (f.key.keysym.sym == SDLK_F9)
										{
											toggle_dt = !toggle_dt;
										}

										if (f.key.keysym.sym == SDLK_F1)
										{
											toggle_help = !toggle_help;

											while (toggle_help) {

												while (SDL_PollEvent(&f) != 0) {

													if (f.type == SDL_KEYDOWN)
													{
														if (f.key.keysym.sym == SDLK_F1)
														{
															toggle_help = false;
														}
													}

												}

												if (s_gamemode == 0 || s_gamemode == 2) {
													SDL_RenderCopy(R, Help_Caeser, NULL, NULL);
												}
												if (s_gamemode == 1 || s_gamemode == 3) {
													SDL_RenderCopy(R, Help_Vignere, NULL, NULL);
												}
												SDL_RenderPresent(R);

											}
										}

										if (f.key.keysym.sym == SDLK_F10)
										{
											if (toggle_bg) {
												toggle_bg = false;
												SDL_SetRenderDrawColor(R, 255, 255, 255, 0xFF);
												UI_Color.r = 0;
												UI_Color.g = 0;
												UI_Color.b = 0;

												NM_Color.r = 0;
												NM_Color.g = 0;
												NM_Color.b = 0;

												for (int i = 0; i < 10; i++) {
													SDL_DestroyTexture(DP_NM[i]);
													NUM = TTF_RenderText_Blended(NUMBERS, to_string(i).c_str(), NM_Color); //Surface = Canvas
													DP_NM[i] = SDL_CreateTextureFromSurface(R, NUM);

													SDL_FreeSurface(NUM);
												}
											}
											else {
												toggle_bg = true;
												SDL_SetRenderDrawColor(R, 0, 0, 0, 0xFF);
												UI_Color.r = 255;
												UI_Color.g = 255;
												UI_Color.b = 255;

												NM_Color.r = 255;
												NM_Color.g = 255;
												NM_Color.b = 255;

												for (int i = 0; i < 10; i++) {
													SDL_DestroyTexture(DP_NM[i]);
													NUM = TTF_RenderText_Blended(NUMBERS, to_string(i).c_str(), NM_Color); //Surface = Canvas
													DP_NM[i] = SDL_CreateTextureFromSurface(R, NUM);

													SDL_FreeSurface(NUM);
												}
											}
										}

										if (f.key.keysym.sym == SDLK_ESCAPE)
										{
											rehash = false;
											playstate = false;
											Mix_HaltMusic();
											//Mix_PlayMusic(Gameplay, -1);
										}
									}

									//cout << SDL_GetError() << " | Polling End " << endl;
								}
								timekeep = time_string(seconds, minutes, hours);
								TIME = TTF_RenderText_Solid(TEXT_S, timekeep.c_str(), UI_Color); //Surface = Canvas
								DP_TM = SDL_CreateTextureFromSurface(R, TIME);
								SDL_QueryTexture(DP_TM, NULL, NULL, &tm->w, &tm->h);
								tm->x = WIDTH / 2 - tm->w / 2; tm->y = 15;

								//cout << SDL_GetError() << " | TimeTexture " << endl;

								if (frames % 60 == 0) {
									for (int i = 0; i < distract; i++) {
										//	cout << "Creating RANDOM NUMBER ART" << endl;

										if (rand() % 2 == 0) {
											n_show[i] = false;
										}
										else {
											n_show[i] = true;
											gen[i] = rand() % 10;
											SDL_QueryTexture(DP_NM[gen[i]], NULL, NULL, &number[i]->w, &number[i]->h);
										}
										//cout << "Created RANDOM NUMBER ART" << endl;
									}
								}

								//Render userinput only when
								if (inputText.length() > 0) {
									ANSWER = TTF_RenderText_Solid(TEXT, inputText.c_str(), UI_Color); //Surface = Canvas
									DP_UI = SDL_CreateTextureFromSurface(R, ANSWER);
									SDL_QueryTexture(DP_UI, NULL, NULL, &ui->w, &ui->h);
									ui->x = WIDTH / 2 - ui->w / 2; ui->y = HEIGHT / 2;
									//cout << SDL_GetError() << " | User Answer " << endl;
									SDL_RenderCopy(R, DP_UI, NULL, ui);
									SDL_FreeSurface(ANSWER);
									SDL_DestroyTexture(DP_UI);
									//cout << SDL_GetError() << " | Render User Answer " << endl;
								}

								//Recents Renderer
								for (int i = 0; i < recent_limit; i++) {
									if (list.at(i) == -1) {
										SDL_RenderCopy(R, NONE, NULL, &rcnts.at(i));
									}
									if (list.at(i) == 0) {
										SDL_RenderCopy(R, WRNG, NULL, &rcnts.at(i));
									}
									if (list.at(i) == 1) {
										SDL_RenderCopy(R, CRCT, NULL, &rcnts.at(i));
									}
								}

								//MAIN RENDERING LINE
								SDL_RenderCopy(R, DP_CT, NULL, name);
								SDL_RenderCopy(R, DP_KY, NULL, key);
								SDL_RenderCopy(R, DP_TM, NULL, tm);
								SDL_RenderCopy(R, DP_TY, NULL, type);
								SDL_RenderCopy(R, DP_HT, NULL, helptext);

								//Change numbers of Cells in Distractions
								if (toggle_dt) {
									for (int i = 0; i < distract; i++) {
										if (frames % 16 == 0) {
											gen[i] = rand() % 10;
										}

										if (n_show[i]) {
											SDL_QueryTexture(DP_NM[gen[i]], NULL, NULL, &number[i]->w, &number[i]->h);
											SDL_RenderCopy(R, DP_NM[gen[i]], NULL, number[i]);
										}
									}
								}

								SDL_RenderPresent(R);

								if (frames % 144 == 0) {
									seconds++;
									if ((seconds) % 60 == 0) {
										minutes++;
										seconds = 0;
										if (minutes % 60 == 0) {
											hours++;
										}
									}
								}

								SDL_RenderPresent(R);
								//cout << SDL_GetError() << " | Start of Texture Cleaning " << endl;

								//Destroy Time Surface and Texture
								SDL_FreeSurface(TIME);
								SDL_DestroyTexture(DP_TM);

								SDL_Delay(7);

								frames++;
							}
						}
						//Mix_HaltMusic();
					}
				}
			}
		}

		SDL_GetMouseState(&(MenuMouse->cursor.x), &(MenuMouse->cursor.y));

		menuUpdate();

		if (button[0]->sel) {
			//SDL_RenderCopy(R, Info_Encrypt, NULL, &Info);
		}
		if (button[1]->sel) {
			//SDL_RenderCopy(R, Info_Decrypt, NULL, &Info);
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

	for (int i = 0; i < 10; i++) {
		SDL_DestroyTexture(DP_NM[i]);
	}

	TTF_CloseFont(TEXT);
	TTF_CloseFont(TEXT_S);
	TTF_CloseFont(TEXT_XS);
	TTF_CloseFont(TEXT_XXS);
	TTF_CloseFont(NUMBERS);
}