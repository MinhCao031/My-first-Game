#include "Constant.h"
#include "MyObj.h"
#include "MoreSDL.h"
#include "GameHandle.h"

int HighScore[3][8], BestTime[3][8], Quest[4], PenaltyTime;
int SCORE, Wait, Limit, MaxEnemy, DeltaWait, Period;
int i, h, diff, Objective, buttonid, IDgame, collision;
bool ForceQuit;

SDL_Event e;
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture *Dots, *Menu, *Unlock, *GameMode, *ChooseALevel, *ChooseNumOfRandom;
SDL_Texture *OBJplus, *OBJcircle, *OBJrandom, *HowToPlay;
TTF_Font *font;
Mix_Music *BGsound[4];
PlayerBox Player;

int main(int argc, char* argv[])
{
    initMedia();
    Goto(renderer, Menu);
    Mix_PlayMusic(BGsound[2], -1);
/**-------------------------------------------------- Game Running, See Info and More ---------------------------------------------------*/
    while (!ForceQuit){
        if ( SDL_PollEvent(&e) == 0) {i = rand(); continue;};
        if (e.type == SDL_QUIT) break;
        if (e.type == SDL_MOUSEBUTTONDOWN){
/* Quit */  if(Click(e,MenuSelect[3])) break;
///------------------------------------------ Unlock-ables --------------------------------------------*/
            if(Click(e,MenuSelect[2])) while (!ForceQuit){
                SDL_RenderCopy(renderer, Unlock, NULL, NULL);
                LoadStat(renderer, font, GetQuestStatus(Quest[0]), Fg, 3 + (Quest[0]>0? 0: 3));
                LoadStat(renderer, font, GetQuestStatus(Quest[1]), Fg, 4 + (Quest[1]>0? 0: 3));
                LoadStat(renderer, font, GetQuestStatus(Quest[2]), Fg, 5 + (Quest[2]>0? 0: 3));
                SDL_RenderPresent(renderer);
/* Waiting */   if ( SDL_WaitEvent(&e) == 0);
/* Force Quit */if (e.type == SDL_QUIT) { ForceQuit = true; break;}
/* Turn Back */ if ( GoBack(e,renderer,Menu) ) break;
                HandleQuest();
            }
///-------------------------------------- See the Instructions ----------------------------------------*/
            if(Click(e,MenuSelect[1])){
                Goto(renderer, HowToPlay);
                while (!ForceQuit){
/* Waiting */       if ( SDL_WaitEvent(&e) == 0);
/* Force Quit */    if (e.type == SDL_QUIT) { ForceQuit = true; break;}
/* Turn Back */     if ( GoBack(e,renderer,Menu) ) break;
                }
            };
///------------------------------------------- Start Game ---------------------------------------------*/
            if(Click(e,MenuSelect[0])){
                Goto(renderer, GameMode);
                while (!ForceQuit){
/* Waiting */       if ( SDL_WaitEvent(&e) == 0);
/* Force Quit */    if (e.type == SDL_QUIT) { ForceQuit = true; break;}
/* Turn Back */     if ( GoBack(e,renderer,Menu) ) break;
/**---------------------------------------------------------- Chose Plus Mode -----------------------------------------------------------*/
                    if (Click(e,GameModeOption[1])){
                        Goto(renderer, ChooseALevel);
                        while (!ForceQuit){
/* Waiting */               if ( SDL_WaitEvent(&e) == 0) ;
/* Force Quit */            if ( e.type == SDL_QUIT) { ForceQuit = true; break;}
/* Turn Back */             if ( GoBack(e,renderer,GameMode) ) break;
                            diff = 0;
                            for( i=1; i<9; i++ ) if(Click(e,HardOption[i])) {diff = i;break;}
/** Chosen Level */         if (diff>0){
                                Goto(renderer, OBJplus); while (!ForceQuit){
/* Waiting */                       if ( SDL_WaitEvent(&e) == 0);
/* Force Quit */                    if ( e.type == SDL_QUIT ) { ForceQuit = true; break;}
/* Turn Back */                     if ( GoBack(e,renderer,ChooseALevel) ) {diff = 0; break;}
                                    Objective = 0;
                                    for( i=1; i<4; i++ ) if(Click(e,Objectives[i])) {Objective = i;break;}
                                    if (Objective == 0) continue;
/** Chosen Objective and Play */    PlayPlus();
/* Play Again or Return to Menu */  if(!ForceQuit) Goto(renderer, GameMode);
                                    break;
                                }
                                if(buttonid > 0 || ForceQuit)
                                {
                                    buttonid = 0;
                                    Mix_PlayMusic(BGsound[2], -1);
                                    break;
                                }
                            }
                        }
                    }
/* End Of PLus Mode */
/**--------------------------------------------------------- Chose Circle Mode ----------------------------------------------------------*/
                    if (Click(e,GameModeOption[2])){
                        Goto(renderer, ChooseALevel); while (!ForceQuit){
/* Waiting */               if ( SDL_WaitEvent(&e) == 0) ;
/* Force Quit */            if (e.type == SDL_QUIT ) { ForceQuit = true; break;}
/* Turn Back */             if ( GoBack(e,renderer,GameMode) ) break;
                            diff = 0;
                            for( i=1; i<9; i++ ) if(Click(e,HardOption[i])) {diff = i;break;}
/** Chosen Level */         if (diff>0){
                                Goto(renderer, OBJcircle); while (!ForceQuit){
/* Waiting */                       if ( SDL_WaitEvent(&e) == 0);
/* Force Quit */                    if ( e.type == SDL_QUIT ) { ForceQuit = true; break;}
/* Turn Back */                     if ( GoBack(e,renderer,ChooseALevel) ) {diff = 0; break;}
                                    Objective = 0;
                                    for( i=1; i<4; i++ ) if(Click(e,Objectives[i])) {Objective = i;break;}
                                    if (Objective == 0) continue;
/** Chosen Objective and Play */    PlayCircle();
/* Play Again or Return to Menu */  if(!ForceQuit) Goto(renderer, GameMode);
                                    break;
                                }
                                if(buttonid > 0 || ForceQuit)
                                {
                                    buttonid = 0;
                                    Mix_PlayMusic(BGsound[2], -1);
                                    break;
                                }
                            }
                        }
                    }
/* End Of Circle Mode */
/**--------------------------------------------------------- Chose Random Mode ----------------------------------------------------------*/
                    if (Click(e,GameModeOption[3])){
                        Goto(renderer, ChooseNumOfRandom); while (!ForceQuit){
/* Waiting */               if ( SDL_WaitEvent(&e) == 0);
/* Force Quit */            if (e.type == SDL_QUIT ) { ForceQuit = true; break;}
/* Turn Back */             if ( GoBack(e,renderer,GameMode) ) break;
                            diff = 0;
                            for( i=1; i<5; i++ ) if(Click(e,NumOfRandom[i])) {diff = i;break;}
/** Chosen Difficulty */    if (diff>0){
                                Goto(renderer, OBJrandom);
                                while (!ForceQuit){
/* Waiting */                       if ( SDL_WaitEvent(&e) == 0);
/* Force Quit */                    if ( e.type == SDL_QUIT ) { ForceQuit = true; break;}
/* Turn Back */                     if ( GoBack(e,renderer,ChooseNumOfRandom) ) {diff = 0; break;}
                                    Objective = 0;
                                    for( i=1; i<4; i++ ) if(Click(e,Objectives[i])) {Objective = i;break;}
                                    if (Objective == 0) continue;
/** Chosen Objective and Play */    PlayRandom();
/* Play Again or Return to Menu */  if(!ForceQuit) Goto(renderer, GameMode);
                                    break;
                                }
                                if(buttonid > 0 || ForceQuit)
                                {
                                    buttonid = 0;
                                    Mix_PlayMusic(BGsound[2], -1);
                                    break;
                                }
                            }
                        }
                    }
/* End Of Random Mode */
                }
            }
        }
    }
/**---------------------------------------------------- End of Loop aka End of Game -----------------------------------------------------*/
    SDL_Delay(100);
    quitSDL(window, renderer);
    if(collision==-10)
    {
        std::cout << "Structure failed!!!!!\n";
        std::cin >> i; // Give me sometime to see the error, nothing else
        return 310000031;
    }
    return 0;
}

///-------------------------------------------------------- End of Main Function --------------------------------------------------------*/
