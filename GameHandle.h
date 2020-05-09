#ifndef GAMEHANDLE_H_INCLUDED
#define GAMEHANDLE_H_INCLUDED

#include "MoreSDL.h"

extern int HighScore[3][8], BestTime[3][8], Quest[4], PenaltyTime;
extern int SCORE, Wait, Limit, MaxEnemy, DeltaWait, Period;
extern int i, h, diff, Objective, buttonid, IDgame, collision;
extern bool ForceQuit;

extern SDL_Event e;
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Texture *Dots, *Menu, *Unlock, *GameMode, *ChooseALevel, *ChooseNumOfRandom;
extern SDL_Texture *OBJplus, *OBJcircle, *OBJrandom, *HowToPlay;
extern TTF_Font *font;
extern Mix_Music *BGsound[4];
extern PlayerBox Player;

// Initialize SDL & Load Media
void initMedia();
// Unlock-able features
int CountTask(int MinDiff);
void HandleQuest();
// Handle the Game
void PlayPlus();
void PlayCircle();
void PlayRandom();

#endif // GAMEHANDLE_H_INCLUDED
