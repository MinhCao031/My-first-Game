#include "MyObj.h"

PlayerBox::PlayerBox()
{
    Size = PLAYER_BOXSIDE;
    x = (SCR_WIDTH - Size)/2;
    y = (SCR_HEIGHT - Size)/2;
    Xpath = ORIGIN_PLAYER_SPD;
    Ypath = ORIGIN_PLAYER_SPD;
}

PlayerBox::~PlayerBox()
{
    Size = 20;
    x = 0; y = 0;
    Xpath = 0; Ypath = 0;
}

void PlayerBox::CircleRender(SDL_Renderer* renderer, SDL_Texture* obj)
{
    SDL_Rect filled_rect;
    filled_rect.x = x;
    filled_rect.y = y;
    filled_rect.w = Size;
    filled_rect.h = Size;
    SDL_RenderCopy(renderer, obj, &SpriteClips[1], &filled_rect);
}

void PlayerBox::PlayerBox::moveRight(short Xstep)
{
    if(x + Xstep < SCR_WIDTH - Size) x += Xstep;
    else x = SCR_WIDTH - Size;
}

void PlayerBox::moveLeft(short Xstep)
{
    if(x > Xstep) x -= Xstep;
    else x = 0;
}

void PlayerBox::moveDown(short Ystep)
{
    if(y + Ystep < SCR_HEIGHT - Size) y += Ystep;
    else y = SCR_HEIGHT - Size;
}

void PlayerBox::moveUp(short Ystep)
{
    if(y > Ystep) y -= Ystep;
    else y = 0;
}

void PlayerBox::FollowCursor(SDL_Event e)
{
 /*
    if (e.button.x <= Size/2) x = 0;
    else if (e.button.x + Size/2 >= SCR_WIDTH) x = SCR_WIDTH - Size;
    else x = e.button.x - Size/2;
    if (e.button.y <= Size/2) y = 0;
    else if (e.button.y + Size/2 >= SCR_HEIGHT) y = SCR_HEIGHT - Size;
    else y = e.button.y - Size/2;
 */
    int X = e.button.x;
    int Y = e.button.y;
    if( X>0 && Y>0 && X < SCR_WIDTH && Y < SCR_HEIGHT )
    {
        if (X > Size/2 && X < SCR_WIDTH - Size/2) x = X - Size/2;
        else if (X + Size/2 <= Size/2) x = 0;
        else if (X >= SCR_WIDTH - Size/2) x = SCR_WIDTH - Size;
        if (Y > Size/2 && Y < SCR_WIDTH - Size) y = Y - Size/2;
        else if (Y + Size/2 <= Size/2) y = 0;
        else if (Y >= SCR_HEIGHT - Size/2) y = SCR_HEIGHT - Size;
    }
}

EnemyBullet::EnemyBullet()
{
    Size = ENEMY_BOXSIDE;
    x = 0; y = 0;
    Speed = 0; xSpeed = 0; ySpeed = 0;
    DSpeed = -0.06; DxSpeed = -0.1; DySpeed = -0.1;
    ID = 0; Status = 1;
}

EnemyBullet::~EnemyBullet()
{
    Size = 0; x = 0; y = 0; ID = 0; Status = 0;
    Speed = 0; xSpeed = 0; ySpeed = 0;
    DSpeed = 0; DxSpeed = 0; DySpeed = 0;
}

bool EnemyBullet::Render(SDL_Renderer* renderer, SDL_Texture* obj)
{
    int X = x;
    int Y = y;
    SDL_Rect filled_rect = {X,Y,Size,Size};
    if( SDL_RenderCopy(renderer, obj, &SpriteClips[0], &filled_rect) < 0 )
    {
        std::cout << "Render Enemy failed!!!\n";
        Status = 8;
        return false;
    };
    return true;
}

bool EnemyBullet::NotCollision(PlayerBox B)
{
    int dx = 2*B.x - 2*x + B.Size - Size;
    int dy = 2*B.y - 2*y + B.Size - Size;
    int dxy = Size + B.Size;
    if( dx*dx + dy*dy < dxy*dxy ) {Status = 7;return false;}
    return true;
}

bool EnemyBullet::IsItOutside()
{
    if(ID>16)
    {
        if(ID==20) if(y < SCR_HEIGHT + 30) return false;
        if(ID==21) if(x < SCR_WIDTH + 30) return false;
        if(ID==22) if(y + Size + 30 > 0) return false;
        if(ID==23) if(x + Size + 30 > 0) return false;
        Status = 6; return true;
    }
    else if(ID<16) // Circle Mode Only
    {
        if(DxSpeed > 0) if(x >= SCR_WIDTH) {Status = 6; return true;}
        if(DxSpeed < 0) if(x + Size <= 0) {Status = 6; return true;}
        if(DySpeed > 0) if(y >= SCR_HEIGHT ) {Status = 6; return true;}
        if(DySpeed < 0) if(y + Size <= 0) {Status = 6; return true;}
        return false;
    }
    else
    {
        std::cout << "Cannot Check!!!\n";
        Status = 8;
        return true;
    }
}

void EnemyBullet::PlusPos(PlayerBox P, int Direction)
{
    if(Status == 1 || Status == 6)
    {
        Status = 2;
        ID = 20 + Direction;
        Speed = ORIGIN_ENEMY_SPD;
        switch(Direction){
            case 0: x = P.x; y = -Size-30; break;
            case 1: x = -Size-30; y = P.y; break;
            case 2: x = P.x; y = SCR_HEIGHT+30; break;
            case 3: x = SCR_WIDTH+30; y = P.y; break;
        }
    }
    else
    {
        std::cout << "PlusPos failed!!!\n";
        Status = 8;
        return;
    }
}

void EnemyBullet::PlusMove(int Score)
{
    int Fast = 4;
    Score /= 10000;
    switch(Score)
    {
        case 0: {Fast = 0; break;}
        case 1: {Fast = 1; break;}
        case 2: {Fast = 2; break;}
        default: break;
    }
    switch(ID)
    {
        case 20: y += (Speed+Fast); return;
        case 21: x += (Speed+Fast); return;
        case 22: y -= (Speed+Fast); return;
        case 23: x -= (Speed+Fast); return;
        default: Status = 8; std::cout << "ID = " << ID << "\nPlusMove failed!!!\n"; return;
    }
}

void EnemyBullet::CircleFirstPos(PlayerBox P,int id)
{
    if(Status == 1 || Status == 5 || Status == 6)
    {
        Status = 2;
        ID = id%16;
        x = P.x + 100*Cos[ID];
        y = P.y + 100*Sin[ID];
        xSpeed = 2*ORIGIN_ENEMY_SPD * Cos[ID];
        ySpeed = 2*ORIGIN_ENEMY_SPD * Sin[ID];
        DSpeed = -0.06;
        DxSpeed = DSpeed * Cos[ID];
        DySpeed = DSpeed * Sin[ID];
        return;
    }
    else
    {
        std::cout << "Status = " << Status << "\nCircleFirstPos failed!!!\n";
        Status = 8;
        return;
    }
}

void EnemyBullet::CircleSecondPos(PlayerBox P)
{
    if(Status == 4)
    {
        Status = 5;
        DSpeed = 0.055;
        double Distance = sqrt( (P.x-x)*(P.x-x) + (P.y-y)*(P.y-y) );
        if (Distance == 0) {Status = 7; return;}
        DxSpeed = (P.x-x) * DSpeed / Distance ;
        DySpeed = (P.y-y) * DSpeed / Distance ;
        return;
    }
    else
    {
        std::cout << "CircleSecondPos failed!!!\n";
        Status = 8;
        return;
    }
}

void EnemyBullet::CircleMove()
{
    if(Status > 1 && Status < 6)
    {
        x += xSpeed;
        y += ySpeed;
        xSpeed += DxSpeed;
        ySpeed += DySpeed;
        if(Status == 2) Status = 3;
        if(xSpeed * Cos[ID] <= 0 || ySpeed * Sin[ID] <= 0)
        {
            if(Status == 3) Status = 4;
            else if(Status != 5) { std::cout << "Status = " << Status << "\nCircleSpeedUp failed!!!\n"; Status = 8; return;}
        }
    }
    else
    {
        std::cout << "Status = " << Status << "\nCircleMove failed!!!\n";
        Status = 8;
        return;
    }
}

void EnemyBullet::RandomPos(int Score)
{
    if(Status == 1 || Status == 6)
    {
        Status = 4;
        ID = 20 + rand()%4;
        if(Score < 400) Speed = 1 + 0.1*(rand()%21); //          1~3.0
            else if(Score < 800)  Speed = 2 + 0.1*(rand()%16);// 2~3.5
            else if(Score < 1200) Speed = 3 + 0.1*(rand()%11);// 3~4.0
            else Speed = 4 + 0.1*(rand()%6); //                  4~4.5
        if(Score < 400) Size = 60 + rand()%61; //               60~120
            else if(Score < 800)  Size = 80 + rand()%51; //     80~130
            else if(Score < 1200) Size = 100 + rand()%41; //   100~140
            else Size = 120 + rand()%31; //                    120~150
        switch(ID)
        {
            case 20:
            {
                x = rand() % SCR_WIDTH - Size/2;
                y = - Size - 30;
                break;
            }
            case 21:
            {
                x = - Size - 30;
                y = rand() % SCR_HEIGHT - Size/2;
                break;
            }
            case 22:
            {
                x = rand() % SCR_WIDTH - Size/2;
                y = SCR_HEIGHT + 30;
                break;
            }
            case 23:
            {
                x = SCR_WIDTH + 30;
                y = rand() % SCR_HEIGHT - Size/2;
                break;
            }
            default: Status = 8; std::cout << "RandomPos failed!!!\n"; return;
        }
    }
    else
    {
        Status = 8;
        std::cout << "RandomPos failed!!!\n";
        return;
    }
}

void EnemyBullet::RandomMove()
{
    switch(ID)
    {
        case 20: y+=Speed; return;
        case 21: x+=Speed; return;
        case 22: y-=Speed; return;
        case 23: x-=Speed; return;
        default: Status = 8; std::cout << "RandomMove failed!!!\n"; return;
    }

}

