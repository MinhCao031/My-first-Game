#include "Constant.h"
#include "MyObj.h"
#include "MoreSDL.h"

int SCORE, MaxEnemy, Wait, Hard, Objective, buttonid;
int i, collision, diff, Limit, DeltaWait, Period;
bool ForceQuit = false;

SDL_Event e;
SDL_Window* window;
SDL_Renderer* renderer = NULL;
PlayerBox Player;

int main(int argc, char* argv[])
{
    initSDL(window, renderer, SCR_WIDTH, SCR_HEIGHT, Title);
    SDL_Texture* Dots = LoadImage(renderer, "dots.png", false, 0, 255, 255);
    SDL_Texture* Menu = LoadImage(renderer, "Menu.png", true);
    SDL_Texture* GameMode = LoadImage(renderer, "GameMode.png", true);
    SDL_Texture* ChooseALevel = LoadImage(renderer, "SelectDifficulty.png", true);
    SDL_Texture* ChooseNumOfRandom = LoadImage(renderer, "SelectNumOfRandom.png", true);
    SDL_Texture* OBJplus = LoadImage(renderer, "ObjectivePlus.png", true);
    SDL_Texture* OBJcircle = LoadImage(renderer, "ObjectiveCircle.png", true);
    SDL_Texture* OBJrandom = LoadImage(renderer, "ObjectiveRandom.png", true);
    SDL_Texture* HowToPlay = LoadImage(renderer, "Instructions.png", true);
    TTF_Font* font = TTF_OpenFont("Segoe Script.ttf", 36);
    Mix_Music *BGsound[] = {
        Mix_LoadMUS("AstronomiaCoffinDance.mp3"),
        Mix_LoadMUS("OutbreakerCut.mp3"),
        Mix_LoadMUS("MenuTheme.mp3"),
        Mix_LoadMUS("EasternYoukaiBeauty.mp3"),
    };

    Goto(renderer, Menu);
    Mix_PlayMusic(BGsound[2], -1);
    while (!ForceQuit){ /* Game Running */
        if ( SDL_WaitEvent(&e) == 0);
        if (e.type == SDL_QUIT) break;
        if (e.type == SDL_MOUSEBUTTONDOWN){
/* Quit */  if(Click(e,MenuSelect[2])) break;
/* See the Instruction */
            if(Click(e,MenuSelect[1])){
                Goto(renderer, HowToPlay);
                while (!ForceQuit){
/* Waiting */       if ( SDL_WaitEvent(&e) == 0);
/* Force Quit */    if (e.type == SDL_QUIT) { ForceQuit = true; break;}
/* Turn Back */     if ( GoBack(e,renderer,Menu) ) break;
                }
            };

/* Start */ if(Click(e,MenuSelect[0])){
                Goto(renderer, GameMode);
                while (!ForceQuit){
/* Waiting */       if ( SDL_WaitEvent(&e) == 0);
/* Force Quit */    if (e.type == SDL_QUIT) { ForceQuit = true; break;}
/* Turn Back */     if ( GoBack(e,renderer,Menu) ) break;
/* Chose anything -----------------------------------------------------------------------------------------------------------------------*/
/* Chose Plus */    if (Click(e,GameModeOption[1])){
                        Goto(renderer, ChooseALevel);
                        while (!ForceQuit){
/* Waiting */               if ( SDL_WaitEvent(&e) == 0) ;
/* Force Quit */            if ( e.type == SDL_QUIT) { ForceQuit = true; break;}
/* Turn Back */             if ( GoBack(e,renderer,GameMode) ) break;
                            diff = 0;
                            for( i=1; i<9; i++ ) if(Click(e,HardOption[i])) {diff = i;break;}
/* Chosen Level */          if (diff>0){
                                Goto(renderer, OBJplus); while (!ForceQuit){
/* Waiting */                       if ( SDL_PollEvent(&e) == 0) {i = rand(); continue;}
/* Force Quit */                    if ( e.type == SDL_QUIT ) { ForceQuit = true; break;}
/* Turn Back */                     if ( GoBack(e,renderer,ChooseALevel) ) {diff = 0; break;}
                                    Objective = 0;
                                    for( i=1; i<4; i++ ) if(Click(e,Objectives[i])) {Objective = i;break;}
                                    if (Objective == 0) continue;
/* Chosen Objective and Play */     do{
/* Setup Everything */                  SCORE = 0; collision = 0; MaxEnemy = 0; buttonid = 0;
                                        SetEnemy(diff, 1, DeltaWait, Limit);
                                        PlayerBox Player; EnemyBullet Plus[Limit];
/* Play Music */                        Mix_PlayMusic(BGsound[3], 20);
/* Start An Attempt */                  for(Wait=-120; collision < 1; Wait+=1){
/* On Plus Mode */                          SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); SDL_RenderClear(renderer);
                                            if (SDL_PollEvent(&e) != 0)
                                            {
/* Force Quit */                                if( e.type == SDL_QUIT ) {std::cout << "pressed Quit\n"; ForceQuit = true; break;}
                                                if( e.key.keysym.sym == SDLK_ESCAPE ) {std::cout << "pressed Esc\n"; break;}
/* Player Move */                               PlayerMove(Player, e, 1);
                                            }
                                            Player.CircleRender(renderer, Dots);
                                            if(Wait >= 0) if( Wait % DeltaWait == 0) if(MaxEnemy<Limit) MaxEnemy+=4;
/* Enemy Move */                            for(i=0;i<MaxEnemy;i++)
                                            {
                                                if(Plus[i].Status==1 || Plus[i].Status==6) Plus[i].PlusPos(Player,i%4);
                                                Plus[i].PlusMove(SCORE);
/* Collision Check */                           if(!Plus[i].NotCollision(Player) ) {collision++;}
                                                if( Plus[i].IsItOutside() ) {
                                                    SCORE += 2160/Limit;
                                                    Plus[i].PlusPos(Player,i%4);
                                                }
                                                if(Plus[i].Status==8)
                                                {
                                                    collision = -10;
                                                    break;
                                                }
                                                Plus[i].Render(renderer,Dots);
                                            }
                                            LoadStat(renderer, font, GetCurrentStat(SCORE, 120+Wait), Fg);
/* Objective Completed? */                  if(Objective < 3) if( SCORE >= 15000*Objective )
                                            {
                                                collision = -Objective;
                                                break;
                                            }
                                            SDL_RenderPresent(renderer);
                                            if(collision == -10) {ForceQuit = true; break;}
                                        }
/* Attempt finished */
                                        SDL_RenderPresent(renderer);
                                        Mix_HaltMusic();
                                        if(!ForceQuit){
/* Play Music */                            Mix_PlayMusic(BGsound[ (collision<0? 1 : 0) ], 1);
                                            char* FinalScore = GetScore(diff,Objective,collision,SCORE);
                                            SDL_MessageBoxData Endgame = {
                                                SDL_MESSAGEBOX_INFORMATION, NULL,
                                                (collision<0? "Congratulation!" : "Game over"),
                                                FinalScore, SDL_arraysize(GameOver),
                                                GameOver, &colorScheme,
                                            };
                                            if (SDL_ShowMessageBox(&Endgame, &buttonid) < 0)  {
                                                SDL_Log("error displaying message box");
                                                ForceQuit = true; break;
                                            }
                                            Mix_HaltMusic();
                                        }
                                    } while (buttonid == 1);
                                    if(!ForceQuit) Goto(renderer, GameMode);
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
/* End Of Game Mode 1 */
/* --------------------------------------------------------------------------------------------------------------------------------------*/
/* Chose Circle */  if (Click(e,GameModeOption[2])){
                        Goto(renderer, ChooseALevel); while (!ForceQuit){
/* Waiting */               if ( SDL_WaitEvent(&e) == 0) ;
/* Force Quit */            if (e.type == SDL_QUIT ) { ForceQuit = true; break;}
/* Turn Back */             if ( GoBack(e,renderer,GameMode) ) break;
                            diff = 0;
                            for( i=1; i<9; i++ ) if(Click(e,HardOption[i])) {diff = i;break;}
/* Chosen Level */          if (diff>0){
                                Goto(renderer, OBJcircle); while (!ForceQuit){
/* Waiting */                       if ( SDL_PollEvent(&e) == 0) {i = rand(); continue;}
/* Force Quit */                    if ( e.type == SDL_QUIT ) { ForceQuit = true; break;}
/* Turn Back */                     if ( GoBack(e,renderer,ChooseALevel) ) {diff = 0; break;}
                                    Objective = 0;
                                    for( i=1; i<4; i++ ) if(Click(e,Objectives[i])) {Objective = i;break;}
                                    if (Objective == 0) continue;
/* Chosen Objective and Play */     do{
/* Setup Everything */                  SetEnemy(diff, 2, DeltaWait, Limit);
                                        SCORE = 0; collision = 0; MaxEnemy = 0; buttonid = 0; Period = DeltaWait * Limit/16;
                                        PlayerBox Player; EnemyBullet Circle[Limit];
/* Play Music */                        Mix_PlayMusic(BGsound[3], 20);
/* Start An Attempt */                  for(Wait=-120; collision < 1; Wait++){
/* On Circle Mode */                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); SDL_RenderClear(renderer);
                                            if (SDL_PollEvent(&e) != 0)
                                            {
/* Force Quit */                                if( e.type == SDL_QUIT ) { ForceQuit = true; break;}
                                                if( e.key.keysym.sym == SDLK_ESCAPE ) break;
/* Player Move */                               PlayerMove(Player, e, 2);
                                            }
                                            Player.CircleRender(renderer, Dots);
                                            if(Wait >= 0) if( Wait % DeltaWait == 0) if(MaxEnemy<Limit) MaxEnemy+=16;
/* Enemy Move */                            for(i=0; i<MaxEnemy; i++)
                                            {
                                                if(Circle[i].Status==1 || Wait%Period == DeltaWait * (i/16) )
                                                    Circle[i].CircleFirstPos(Player,i);
                                                if(Circle[i].Status < 6)
                                                {
                                                    Circle[i].CircleMove();
/* Collision Check */                               if(Circle[i].NotCollision(Player) == false) {collision++;}
                                                    if(Circle[i].Status == 4) Circle[i].CircleSecondPos(Player);
                                                    if(Circle[i].IsItOutside() == true) SCORE++;
                                                }
                                                if(Circle[i].Status==8)
                                                {
                                                    collision = -10;
                                                    break;
                                                }
                                                Circle[i].Render(renderer,Dots);
                                            }
                                            LoadStat(renderer, font, GetCurrentStat(SCORE, 120+Wait), Fg);
                                            SDL_RenderPresent(renderer);
/* Objective Completed? */                  if(Objective < 3) if(100 + Wait >= 1800*Objective*Objective)
                                            {
                                                collision = -Objective;
                                                break;
                                            }
                                            if(collision == -10) {ForceQuit = true; break;}
                                        }
/* Attempt finished */
                                        SDL_RenderPresent(renderer);
                                        Mix_HaltMusic();
                                        if(!ForceQuit){
/* Play Music */                            Mix_PlayMusic(BGsound[ (collision<0? 1 : 0) ], 1);
                                            char* FinalScore = GetScore(diff,Objective,collision,SCORE);
                                            SDL_MessageBoxData Endgame = {
                                                 SDL_MESSAGEBOX_INFORMATION, NULL,
                                                (collision<0? "Congratulation!" : "Game over"),
                                                FinalScore, SDL_arraysize(GameOver), GameOver, &colorScheme,
                                            };
                                            if (SDL_ShowMessageBox(&Endgame, &buttonid) < 0)  {
                                            SDL_Log("error displaying message box");
                                            ForceQuit = true; break;
                                            }
                                            Mix_HaltMusic();
                                        }
                                    } while (buttonid == 1);
                                    if(!ForceQuit) Goto(renderer, GameMode);
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
/* End Of Game Mode 2 */
/* --------------------------------------------------------------------------------------------------------------------------------------*/
/* Chose Random */  if (Click(e,GameModeOption[3])){
                        Goto(renderer, ChooseNumOfRandom); while (!ForceQuit){
/* Waiting */               if ( SDL_WaitEvent(&e) == 0) ;
/* Force Quit */            if (e.type == SDL_QUIT ) { ForceQuit = true; break;}
/* Turn Back */             if ( GoBack(e,renderer,GameMode) ) break;
                            diff = 0;
                            for( i=1; i<5; i++ ) if(Click(e,NumOfRandom[i])) {diff = i;break;}
/* Chosen Level */          if (diff>0){
                                Goto(renderer, OBJrandom);
                                while (!ForceQuit){
/* Waiting */                       if ( SDL_PollEvent(&e) == 0) {i = rand(); continue;};
/* Force Quit */                    if ( e.type == SDL_QUIT ) { ForceQuit = true; break;}
/* Turn Back */                     if ( GoBack(e,renderer,ChooseNumOfRandom) ) {diff = 0; break;}
                                    Objective = 0;
                                    for( i=1; i<4; i++ ) if(Click(e,Objectives[i])) {Objective = i;break;}
                                    if (Objective == 0) continue;
/* Chosen Objective and Play */     do {
/* Setup Everything */                  SetEnemy(diff, 3, DeltaWait, Limit);
                                        SCORE = 0; collision = 0; buttonid = 0;
                                        PlayerBox Player; EnemyBullet Random[Limit];
/* Play Music */                        Mix_PlayMusic(BGsound[3], 20);
/* Start an attempt */                  for(Wait=-120; collision < 1; Wait++){
/* On Random Mode */                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); SDL_RenderClear(renderer);
                                            if (SDL_PollEvent(&e) != 0)
                                            {
/* Force Quit */                                if( e.type == SDL_QUIT ) { ForceQuit = true; break;}
                                                if( e.key.keysym.sym == SDLK_ESCAPE ) break;
/* Player Move */                               PlayerMove(Player, e, 2);
                                            }
                                            Player.CircleRender(renderer, Dots);
/* Enemy Move */                            if(Wait>=0) for(i=0;i<Limit;i++)
                                            {
                                                if(Wait==0) Random[i].RandomPos(0);
                                                Random[i].Render(renderer, Dots);
                                                Random[i].RandomMove();
/* Collision Check */                           if(!Random[i].NotCollision(Player) ) collision++;
                                                if( Random[i].IsItOutside() ) {SCORE+=3; Random[i].RandomPos(SCORE);}
                                                if( Random[i].Status == 8) {collision = -10; break;}
                                            }
                                            LoadStat(renderer, font, GetCurrentStat(SCORE, 120+Wait), Fg);
/* Objective Completed? */                  if(Objective < 3) if( SCORE >= 400*Objective + 200 )
                                            {
                                                collision = -Objective;
                                                break;
                                            }
                                            SDL_RenderPresent(renderer);
                                            if(collision == -10) {ForceQuit = true; break;}
                                        }
/* Attempt Finished */
                                        SDL_RenderPresent(renderer);
                                        Mix_HaltMusic();
                                        if(!ForceQuit){
/* Play Music */                            Mix_PlayMusic(BGsound[ (collision<0? 1 : 0) ], 1);
                                            char* FinalScore = GetScore(diff,Objective,collision,SCORE);
                                            SDL_MessageBoxData Endgame = {
                                                 SDL_MESSAGEBOX_INFORMATION, NULL,
                                                (collision<0? "Congratulation!" : "Game over"),
                                                FinalScore, SDL_arraysize(GameOver), GameOver, &colorScheme,
                                            };
                                            if (SDL_ShowMessageBox(&Endgame, &buttonid) < 0)  {
                                            SDL_Log("error displaying message box");
                                            ForceQuit = true; break;
                                            }
                                            Mix_HaltMusic();
                                        }
                                    } while (buttonid == 1);
                                    if(!ForceQuit) Goto(renderer, GameMode);
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
/* End Of Game Mode 3 */
/* --------------------------------------------------------------------------------------------------------------------------------------*/
                }
            }
        }
    }

    SDL_Delay(100);
    quitSDL(window, renderer);
    if(collision==-10)
    {
        std::cout << "Structure failed!!!!!!!!!!!!!!!!\n";
        std::cin >> i;
        return 1234;
    }
    return 0;
}

// End of Main Function
