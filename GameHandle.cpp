#include "GameHandle.h"

void initMedia()
{
    h=1; ForceQuit = false;
    initSDL(window, renderer, SCR_WIDTH, SCR_HEIGHT, Title);

    Dots = loadImage(renderer, "dots.png", false, 0, 255, 255);
    Menu = loadImage(renderer, "Menu.png", true);
    Unlock =  loadImage(renderer, "Unlock.png", true);
    GameMode = loadImage(renderer, "GameMode.png", true);
    ChooseALevel = loadImage(renderer, "SelectDifficulty.png", true);
    ChooseNumOfRandom = loadImage(renderer, "SelectNumOfRandom.png", true);
    OBJplus = loadImage(renderer, "ObjectivePlus.png", true);
    OBJcircle = loadImage(renderer, "ObjectiveCircle.png", true);
    OBJrandom = loadImage(renderer, "ObjectiveRandom.png", true);
    HowToPlay = loadImage(renderer, "Instructions.png", true);

    font = TTF_OpenFont("Segoe Script.ttf", 36);
    BGsound[0] = Mix_LoadMUS("AstronomiaCoffinDance.mp3");
    BGsound[1] = Mix_LoadMUS("OutbreakerCut.mp3");
    BGsound[2] = Mix_LoadMUS("MenuTheme.mp3");
    BGsound[3] = Mix_LoadMUS("EasternYoukaiBeauty.mp3");
}

int CountTask(int MinDiff)
{
    int Ans=0;
    for(int j = MinDiff/2; j<4; j++) if ( HighScore[0][j] > 599)
    {
        Ans++; break;
    }
    for(int j = MinDiff - 1; j<8; j++) if ( HighScore[1][j] > 14999)
    {
        Ans++; break;
    }
    for(int j = MinDiff - 1; j<8; j++) if ( BestTime[2][j] > 3599)
    {
        Ans++; break;
    }
    return Ans;
}

void HandleQuest()
{
    //int Check[3] = {CountTask(3), CountTask(4), CountTask(5)};
    int *Checking = new int[3];
    *Checking = CountTask(3);
    *(Checking+1) = CountTask(4);
    *(Checking+2) = CountTask(5);
    if (Click(e,GameModeOption[0])) h *= -1;
    for(int k=0; k<3; k++) if (Click(e,UNLOCK[k])){
        if(Quest[k] == 0){
            if( h<0 || *(Checking+k) > h ){
                Quest[k] = 2;
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Quest Completed", "Unlock successfully!",NULL);
            }
            else SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Locked", GetQuest(k),NULL);
        }
        else if(Quest[k] == 1) Quest[k] = 2;
        else if(Quest[k] == 2) Quest[k] = 1;
    }
    delete Checking;
}

/**------------------------------------------------------------------------- Play Plus -------------------------------------------------------------------------*/
void PlayPlus()
{
/* Chosen Objective and Play */ do{
/* Setup Everything */              SCORE = 0; collision = 0; MaxEnemy = 0; buttonid = 0; IDgame = diff - 1;
                                    SetEnemy(diff, 1, DeltaWait, Limit);
                                    PlayerBox Player; EnemyBullet Plus[Limit];
                                    if(Quest[2] == 2) Player.FollowCursor(e);
/* Play Music */                    Mix_PlayMusic(BGsound[3], 20);
/* Start An Attempt */              for(Wait=-120; collision < 1; Wait+=1){
/* Clear Screen */                      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); SDL_RenderClear(renderer);
                                        if (SDL_PollEvent(&e) != 0)
                                        {
/* Force Quit */                            if( e.type == SDL_QUIT ) {std::cout << "pressed Quit\n"; ForceQuit = true; break;}
/* Player Move */                           PlayerMove(Player, e, 1, Quest);
                                        }
                                        Player.CircleRender(renderer, Dots);
                                        if(Wait >= 0) if( Wait % DeltaWait == 0) if(MaxEnemy<Limit) MaxEnemy+=4;
/* Enemy Move */                        for(i=0;i<MaxEnemy;i++)
                                        {
                                            if(Plus[i].Status==1 || Plus[i].Status==6) Plus[i].PlusPos(Player,i%4);
                                            Plus[i].PlusMove(SCORE);
/* Collision Check */                       if(!Plus[i].NotCollision(Player) ) {collision++;}
                                            if( Plus[i].IsItOutside() ) {
                                                SCORE += 2160/Limit;
                                                Plus[i].PlusPos(Player,i%4);
                                            }
/* Emergency in Case of Error */            if(Plus[i].Status==8)
                                            {
                                                collision = -10;
                                                break;
                                            }
                                            Plus[i].Render(renderer,Dots);
                                        }
                                        LoadStat(renderer, font, GetCurrentStat(SCORE, 120+Wait), Fg);
                                        LoadStat(renderer, font, GetCurrentBestStat(HighScore[1][IDgame], BestTime[1][IDgame]), Fg, 1);
                                        LoadStat(renderer, font, CurrentMode(1,diff), Fg, 2);
                                        SDL_RenderPresent(renderer);
/* Objective Completed? */              if(Objective < 3) if( SCORE >= 15000*Objective )
                                        {
                                            collision = -Objective;
                                            break;
                                        }
/* Game Paused */                       if(e.key.keysym.sym == SDLK_p && (e.button.y == 1978138624 || e.button.y == 0)) // (e.button.x == 112 && e.button.y == 1978138624)
                                        {
                                            // std::cout << e.button.x << " " << e.button.y << "\n";
                                            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING,"Game Paused",
                                                "Penalty: -500 points.\nPress Enter to continue playing.",NULL);
/* Penalty */                               SCORE = (SCORE > 500? SCORE - 500 : 0);
                                        }
/* Emergency in Case of Error */        if(collision == -10) {ForceQuit = true; break;}
/* Attempt finished */              }
/* Check High Score & Best Time */  UpdateBestStat(SCORE, Wait, HighScore[1][IDgame], BestTime[1][IDgame]);
                                    Mix_PausedMusic(); Mix_HaltMusic();
                                    if(!ForceQuit){
/* Play Game over Music */              Mix_PlayMusic(BGsound[ (collision<0? 1 : 0) ], 1);
                                        char* FinalScore = GetScore(diff, Objective, collision, SCORE);
                                        SDL_MessageBoxData Endgame = {
                                            SDL_MESSAGEBOX_INFORMATION, NULL,
                                            (collision<0? "Congratulation!" : "Game over"),
                                            FinalScore, SDL_arraysize(GameOver),
                                            GameOver, &colorScheme,
                                        };
/* Show Message Box */                  if (SDL_ShowMessageBox(&Endgame, &buttonid) < 0)  {
                                            SDL_Log("error displaying message box");
                                            ForceQuit = true; break;
                                        }
                                        Mix_PausedMusic(); Mix_HaltMusic();
                                        delete []FinalScore;
                                    }
                                } while (buttonid == 1);
}

/**------------------------------------------------------------------------ Play Circle ------------------------------------------------------------------------*/
void PlayCircle()
{
/* Chosen Objective and Play */ do{
/* Setup Everything */              SetEnemy(diff, 2, DeltaWait, Limit);
                                    SCORE = 0; collision = 0; MaxEnemy = 0; Period = DeltaWait * Limit/16;
                                    IDgame = diff - 1; buttonid = 0; PenaltyTime = 0;
                                    PlayerBox Player; EnemyBullet Circle[Limit];
                                    if(Quest[2] == 2) Player.FollowCursor(e);
/* Play Music */                    Mix_PlayMusic(BGsound[3], 20);
/* Start An Attempt */              for(Wait=-120; collision < 1; Wait++){
/* Clear Screen */                      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); SDL_RenderClear(renderer);
                                        if (SDL_PollEvent(&e) != 0)
                                        {
/* Force Quit */                            if( e.type == SDL_QUIT ) {std::cout << "pressed Quit\n"; ForceQuit = true; break;}
/* Player Move */                           PlayerMove(Player, e, 2, Quest);
                                        }
                                        Player.CircleRender(renderer, Dots);
                                        if(Wait >= 0) if( Wait % DeltaWait == 0) if(MaxEnemy<Limit) MaxEnemy+=16;
/* Enemy Move */                        for(i=0; i<MaxEnemy; i++)
                                        {
                                            if(Circle[i].Status==1 || Wait%Period == DeltaWait * (i/16) )
                                                Circle[i].CircleFirstPos(Player,i);
                                            if(Circle[i].Status < 6)
                                            {
                                                Circle[i].CircleMove();
/* Collision Check */                           if(Circle[i].NotCollision(Player) == false) {collision++;}
                                                if(Circle[i].Status == 4) Circle[i].CircleSecondPos(Player);
                                                if(Circle[i].IsItOutside() == true) SCORE++;
                                            }
/* Emergency in Case of Error */            if(Circle[i].Status==8)
                                            {
                                                collision = -10;
                                                break;
                                            }
                                            Circle[i].Render(renderer,Dots);
                                        }
                                        LoadStat(renderer, font, GetCurrentStat(SCORE, 120+Wait-PenaltyTime), Fg);
                                        LoadStat(renderer, font, GetCurrentBestStat(HighScore[2][IDgame], BestTime[2][IDgame]), Fg, 1);
                                        LoadStat(renderer, font, CurrentMode(2,diff), Fg, 2);
                                        SDL_RenderPresent(renderer);
/* Objective Completed? */              if(Objective < 3) if(120 + Wait - PenaltyTime >= 1800*Objective*Objective)
                                        {
                                            collision = -Objective;
                                            break;
                                        }
/* Game Paused */                       if(e.key.keysym.sym == SDLK_p && (e.button.y == 1978138624 || e.button.y == 0)) // (e.button.x == 112 && e.button.y == 1978138624)
                                        {
                                            // std::cout << e.button.x << " " << e.button.y << "\n";
                                            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Game Paused.",
                                                "Penalty: -2 seconds.\nPress Enter to continue playing.", NULL);
/* Penalty */                               PenaltyTime += (Wait-PenaltyTime > 0? 120 : Wait+120-PenaltyTime);
                                        }
/* Emergency in Case of Error */        if(collision == -10) {ForceQuit = true; break;}
/* Attempt finished */              }
/* Check High Score & Best Time */  UpdateBestStat(SCORE, Wait-PenaltyTime, HighScore[2][IDgame], BestTime[2][IDgame]);
                                    Mix_PausedMusic(); Mix_HaltMusic();
                                    if(!ForceQuit){
/* Play Music */                        Mix_PlayMusic(BGsound[ (collision<0? 1 : 0) ], 1);
                                        char* FinalScore = GetScore(diff,Objective,collision,SCORE);
                                        SDL_MessageBoxData Endgame = {
                                            SDL_MESSAGEBOX_INFORMATION, NULL,
                                            (collision<0? "Congratulation!" : "Game over"),
                                            FinalScore, SDL_arraysize(GameOver), GameOver, &colorScheme,
                                        };
/* Show Message Box */                  if (SDL_ShowMessageBox(&Endgame, &buttonid) < 0)  {
                                            SDL_Log("error displaying message box");
                                            ForceQuit = true; break;
                                        }
                                        Mix_PausedMusic(); Mix_HaltMusic();
                                        delete []FinalScore;
                                    }
                                } while (buttonid == 1);
}

/**------------------------------------------------------------------------ Play Random ------------------------------------------------------------------------*/
void PlayRandom()
{
/* Chosen Objective and Play */ do {
/* Setup Everything */              SetEnemy(diff, 3, DeltaWait, Limit);
                                    SCORE = 0; collision = 0; buttonid = 0; IDgame = diff - 1;
                                    PlayerBox Player; EnemyBullet Random[Limit];
                                    if(Quest[2] == 2) Player.FollowCursor(e);
/* Play Music */                    Mix_PlayMusic(BGsound[3], 20);
/* Start an attempt */              for(Wait=-120; collision < 1; Wait++){
/* Clear Screen */                      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); SDL_RenderClear(renderer);
                                        if (SDL_PollEvent(&e) != 0)
                                        {
/* Force Quit */                            if( e.type == SDL_QUIT ) {std::cout << "pressed Quit\n"; ForceQuit = true; break;}
/* Player Move */                           PlayerMove(Player, e, 2, Quest);
                                        }
                                        Player.CircleRender(renderer, Dots);
/* Enemy Move */                        if(Wait>=0) for(i=0;i<Limit;i++)
                                        {
                                            if(Wait==0) Random[i].RandomPos(0);
                                            Random[i].Render(renderer, Dots);
                                            Random[i].RandomMove();
/* Collision Check */                       if(!Random[i].NotCollision(Player) ) collision++;
                                            if( Random[i].IsItOutside() ) {SCORE+=3; Random[i].RandomPos(SCORE);}
/* Emergency in Case of Error */            if( Random[i].Status == 8) {collision = -10; break;}
                                        }
                                        LoadStat(renderer, font, GetCurrentStat(SCORE, 120+Wait), Fg);
                                        LoadStat(renderer, font, GetCurrentBestStat(HighScore[0][IDgame], BestTime[0][IDgame]), Fg, 1);
                                        LoadStat(renderer, font, CurrentMode(0,diff), Fg, 2);
                                        SDL_RenderPresent(renderer);
/* Objective Completed? */              if(Objective < 3) if( SCORE >= 400*Objective + 200 )
                                        {
                                            collision = -Objective;
                                            break;
                                        }
/* Game Paused */                       if(e.key.keysym.sym == SDLK_p && (e.button.y == 1978138624 || e.button.y == 0)) // (e.button.x == 112 && e.button.y == 1978138624)
                                        {
                                            // std::cout << e.button.x << " " << e.button.y << "\n";
                                            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING,"Game Paused",
                                                "Penalty: -20 points.\nPress Enter to continue playing.",NULL);
/* Penalty */                               SCORE = (SCORE > 20? SCORE - 20 : 0);
                                        }
/* Emergency in Case of Error */        if(collision == -10) {ForceQuit = true; break;}
 /* Attempt Finished */             }
/* Check High Score & Best Time */  UpdateBestStat(SCORE, Wait, HighScore[0][IDgame], BestTime[0][IDgame]);
                                    Mix_PausedMusic(); Mix_HaltMusic();
                                    if(!ForceQuit){
/* Play Music */                        Mix_PlayMusic(BGsound[ (collision<0? 1 : 0) ], 1);
                                        char* FinalScore = GetScore(diff,Objective,collision,SCORE);
                                        SDL_MessageBoxData Endgame = {
                                            SDL_MESSAGEBOX_INFORMATION, NULL,
                                            (collision<0? "Congratulation!" : "Game over"),
                                            FinalScore, SDL_arraysize(GameOver), GameOver, &colorScheme,
                                        };
/* Show Message Box */                  if (SDL_ShowMessageBox(&Endgame, &buttonid) < 0)  {
                                            SDL_Log("error displaying message box");
                                            ForceQuit = true; break;
                                        }
                                        Mix_PausedMusic(); Mix_HaltMusic();
                                        delete []FinalScore;
                                    }
                                } while (buttonid == 1);
}

/**------------------------------------------------------------------------ End of File ------------------------------------------------------------------------*/
