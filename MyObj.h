#ifndef MYOBJ_H_INCLUDED
#define MYOBJ_H_INCLUDED

#include <math.h>
#include "Constant.h"

struct PlayerBox{
    double x,y;
    short Xpath,Ypath,Size;
// Initialize, Deallocate and Render
    PlayerBox();
    ~PlayerBox();
    void CircleRender(SDL_Renderer* renderer, SDL_Texture* obj);
// Player Movement
    void moveRight(short Xstep);
    void moveLeft(short Xstep);
    void moveDown(short Ystep);
    void moveUp(short Ystep);
};

class EnemyBullet{
    double x, y;
    double Speed, xSpeed, ySpeed;
    double DSpeed, DxSpeed, DySpeed;
    short ID, Size;
public:
    signed char Status;
// Initialize and Deallocate
    EnemyBullet();
    ~EnemyBullet();
// Function for ALL Mode
    bool Render(SDL_Renderer* renderer, SDL_Texture* obj);
    bool NotCollision(PlayerBox B);
    bool IsItOutside();
// Plus Mode
    void PlusPos(PlayerBox P, int Direction);
    void PlusMove(int Score);
// Circle Mode
    void CircleFirstPos(PlayerBox P,int id);
    void CircleSecondPos(PlayerBox P);
    void CircleMove();
// Random Mode
    void RandomPos(int Score);
    void RandomMove();
};

/* Structure Above
    Status
 = 0 : Not Initialized
 = 1 : Initialized First Time Only
 = 2 : Setup Start Position (Due to Moving Out or First Initialized)
 = 3 : Moving Slowly Overtime (Circle Mode Only)
 = 4 : Temporary Stopped (Circle Mode) or Constant Motion (Other Mode)
 = 5 : Moving Fast Overtime To Chase Player (Circle Mode Only)
 = 6 : Out of Window
 = 7 : Collided
 = 8 : an Unexpected Error Occurs
    ID
 = 0~15 (Circle Mode Only) Angle Start
 = 21~24 (Other Mode) Edge Start
*/

#endif // MYOBJ_H_INCLUDED
