#ifndef MORESDL_H_
#define MORESDL_H_

#include <iostream>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "Constant.h"
#include "MyObj.h"

void logSDLError(std::ostream& os, const std::string &msg, bool fatal);

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer, int screenWidth, int screenHeight, const char* windowTitle);

void quitSDL(SDL_Window* window, SDL_Renderer* renderer);

void LoadStat(SDL_Renderer *RENDERER, TTF_Font *font, char* Stat, SDL_Color fg);

SDL_Texture* LoadImage(SDL_Renderer *RENDERER, std::string path, bool KeepBG, int red = 0, int green = 0, int blue = 0);

bool Click(SDL_Event E, SDL_Rect Area);

bool GoBack(SDL_Event E, SDL_Renderer* renderer, SDL_Texture* where);

bool WantToBack(SDL_Texture* where, SDL_Renderer* renderer, SDL_Event E, bool& Quit);

void Goto(SDL_Renderer* renderer, SDL_Texture* where);

void SetEnemy(int DIFF, int BulletKind, int& DeltaWait, int& Limit);

void PlayerMove(PlayerBox& Player, SDL_Event e, int Mode);

char* GetCurrentStat(int Score, int Wait);

char* GetScore(int DIFF, int MODE, int COLL, int SCORE);

#endif // MORESDL_H_INCLUDED
