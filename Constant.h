#ifndef CONSTANT_H_INCLUDED
#define CONSTANT_H_INCLUDED

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

const int SCR_WIDTH = 1000;
const int SCR_HEIGHT = 1000;
const int PLAYER_BOXSIDE = 20;
const int ENEMY_BOXSIDE = 20;
const int ORIGIN_PLAYER_SPD = 8;
const double ORIGIN_ENEMY_SPD = 2;
const char Title[] = "Dodge the Bullets 1.0 by MinhCao";

const double Sin[16] = {
    0.1950903220,  0.5555702330,  0.8314696123,  0.9807852804,
    0.9807852804,  0.8314696123,  0.5555702330,  0.1950903220,
   -0.1950903220, -0.5555702330, -0.8314696123, -0.9807852804,
   -0.9807852804, -0.8314696123, -0.5555702330, -0.1950903220,
}; // sin( (i+0.5)*pi/16 ), i=0->15;

const double Cos[16] = {
    0.9807852804,  0.8314696123,  0.5555702330,  0.1950903220,
   -0.1950903220, -0.5555702330, -0.8314696123, -0.9807852804,
   -0.9807852804, -0.8314696123, -0.5555702330, -0.1950903220,
    0.1950903220,  0.5555702330,  0.8314696123,  0.9807852804,
}; // cos( (i+0.5)*pi/16 ), i=0->15;

const SDL_Rect MenuSelect[] = {
    {500, 100, 400, 200}, // Game Start
    {500, 400, 400, 100}, // Instruction
    {500, 600, 400, 100}, // Unlock-ables
    {500, 800, 400, 100}, // Quit
};

const SDL_Rect Instruction = {100, 100, 800, 800};
const SDL_Rect Back = {50, 50, 100, 50};

const SDL_Rect UNLOCK[] = {
    {750, 250, 200, 100},
    {750, 500, 200, 100},
    {750, 750, 200, 100},
};

const SDL_Rect GameModeOption[] = {
    {  5,   5,   5,   5}, // Hack
    {100, 200, 400, 100}, // Plus-Homing
    {100, 500, 400, 100}, // Circle-Homing
    {100, 800, 400, 100}, // Random
};

const SDL_Rect NumOfRandom[] = {
    { 50,  50, 100,  50}, // Back
    {100, 200, 200, 100}, // 16
    {100, 400, 200, 100}, // 32
    {100, 600, 200, 100}, // 48
    {100, 800, 200, 100}, // 64
};

const SDL_Rect HardOption[] = {
    { 50,  50, 100,  50}, // Back
    {100, 200, 250, 100}, // 1
    {650, 200, 250, 100}, // 2
    {100, 400, 250, 100}, // 3
    {650, 400, 250, 100}, // 4
    {100, 600, 250, 100}, // 5
    {650, 600, 250, 100}, // 6
    {100, 800, 250, 100}, // 7
    {650, 800, 250, 100}, // 8
};

const SDL_Rect Objectives[] = {
    { 50,  50, 100,  50}, // Back
    {200, 200, 600, 150}, // obj1
    {200, 450, 600, 150}, // obj2
    {200, 700, 600, 150}, // obj3 - infinity
};

const SDL_Rect SpriteClips[4] = {
    {0,   0, 100, 100},
    {0, 100, 100, 100},
  {100,   0, 100, 100},
  {100, 100, 100, 100},
};

const SDL_Rect SetNewBestStat = {0, SCR_HEIGHT - 50, SCR_WIDTH, 50};
const SDL_Color Fg = { 0, 255, 255};

const SDL_MessageBoxColorScheme colorScheme = {
    { /* .colors (.r, .g, .b) */
        /* [SDL_MESSAGEBOX_COLOR_BACKGROUND]        */ { 255,   0,   0 },
        /* [SDL_MESSAGEBOX_COLOR_TEXT]              */ {   0, 255,   0 },
        /* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER]     */ { 255,   0,   0 },
        /* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */ { 255,   0,   0 },
        /* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED]   */ { 255,   0,   0 },
    }
};

const SDL_MessageBoxButtonData GameOver[] = {
        { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 2, "Back to Menu." },
        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Play Again." },
};

#endif // CONSTANT_H_INCLUDED
