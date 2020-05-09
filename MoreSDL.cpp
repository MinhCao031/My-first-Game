#include "MoreSDL.h"

void logSDLError(std::ostream& os, const std::string &msg, bool fatal)
{
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(31);
    }
}

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer, int screenWidth, int screenHeight, const char* windowTitle)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) logSDLError(std::cout, "SDL_Init", true);

    window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, 25, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    if (window == NULL) logSDLError(std::cout, "CreateWindow", true);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
//For WinXP or virtual Win: renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));

    if (renderer == NULL) logSDLError(std::cout, "CreateRenderer", true);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, screenWidth, screenHeight);

    TTF_Init();
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	TTF_Quit();
	Mix_CloseAudio();
}

void LoadStat(SDL_Renderer *RENDERER, TTF_Font *font, char* Stat, SDL_Color fg, int AskText)
{
    //char* Stat = GetCurrentStat(SCORE, 120+Wait);
    SDL_Surface *S = TTF_RenderText_Solid(font, Stat, fg);
    if(S == NULL) std::cout << "#1: " << SDL_GetError() << "\n";
    SDL_Texture *T = SDL_CreateTextureFromSurface(RENDERER, S);
    if(T == NULL) std::cout << "#2: " << SDL_GetError() << "\n";
    SDL_Rect Location = {0, 0, 0, 0};
    switch(AskText)
    {
        case 1: Location.y = SCR_HEIGHT - 50; break;
        case 2: Location.y = 50; break;
        case 3: Location.x = 750+70; Location.y = 250+20; break;
        case 4: Location.x = 750+70; Location.y = 500+20; break;
        case 5: Location.x = 750+70; Location.y = 750+20; break;
        case 6: Location.x = 750+30; Location.y = 250+20; break;
        case 7: Location.x = 750+30; Location.y = 500+20; break;
        case 8: Location.x = 750+30; Location.y = 750+20; break;
        default: break;
    }
    TTF_SizeText(font, Stat, &Location.w, &Location.h);
    if(SDL_RenderCopy(RENDERER, T, NULL, &Location)<0)
        std::cout << "#3: " << SDL_GetError() << "\nText is: \"" << Stat << "\"<endstring> \n";
    SDL_FreeSurface(S);
    SDL_DestroyTexture(T);
}
/*
void LoadStatus(SDL_Renderer *RENDERER, TTF_Font *font, char* Stat, SDL_Color fg, int QuestNum)
{
    SDL_Surface *S = TTF_RenderText_Solid(font, Stat, fg);
    if(S == NULL) std::cout << "#4: " << SDL_GetError() << "\n";
    SDL_Texture *T = SDL_CreateTextureFromSurface(RENDERER, S);
    if(T == NULL) std::cout << "#5: " << SDL_GetError() << "\n";
    SDL_Rect Location = {750, 250, 0, 0};
    switch(QuestNum)
    {
        case 0: Location.y = 250; break;
        case 1: Location.y = 500; break;
        case 2: Location.y = 750; break;
        default: break;
    }
    TTF_SizeText(font, Stat, &Location.w, &Location.h);
    if(SDL_RenderCopy(RENDERER, T, NULL, &Location)<0)
        std::cout << "#6: " << SDL_GetError() << "\nText is: \"" << Stat << "\"<endstring> \n";
    SDL_FreeSurface(S);
    SDL_DestroyTexture(T);
}
*/
SDL_Texture* loadImage(SDL_Renderer *RENDERER, std::string path, bool KeepBG,int red, int green, int blue)
{
//The final texture
	SDL_Texture* newTexture = NULL;
//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL ) std::cout << "Unable to load image " << path.c_str() << "! SDL_image Error: " << IMG_GetError() << "\n";
	else{
    //Remove BackGround if necessary
        if( !KeepBG ) SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, red, green, blue) );
    //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(RENDERER, loadedSurface );
		if( newTexture == NULL ) std::cout << "Unable to create texture from " << path.c_str() << "! SDL Error: " << IMG_GetError() << "\n";
    //Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}
	return newTexture;
}

bool Click(SDL_Event E, SDL_Rect Area)
{
    if (E.type != SDL_MOUSEBUTTONDOWN) return false;
    if (E.button.x > Area.x && E.button.x < Area.x + Area.w)
        if (E.button.y > Area.y && E.button.y < Area.y + Area.h) return true;
    return false;
}

bool GoBack(SDL_Event E, SDL_Renderer* renderer, SDL_Texture* where)
{
    if (Click(E,Back))
    {
        Goto(renderer, where);
        return true;
    }
    return false;

}

bool WantToBack(SDL_Texture* where, SDL_Renderer* renderer, SDL_Event E, bool& Quit)
{
    if( SDL_WaitEvent(&E) != 0 )
    {
        if ( E.type == SDL_QUIT) { Quit = true; return true;}
        if ( GoBack(E,renderer,where) ) return true;
        return false;
    }
    return false;
}

void Goto(SDL_Renderer* renderer, SDL_Texture* where)
{
    SDL_RenderCopy(renderer, where, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void SetEnemy(int DIFF, int BulletKind, int& DeltaWait, int& Limit)
{
    if(BulletKind == 1)
    {
        switch(DIFF)
        {
        case 1: DeltaWait = 36; Limit = 4*9; break;
        case 2: DeltaWait = 36; Limit = 4*12; break;
        case 3: DeltaWait = 36; Limit = 4*15; break;
        case 4: DeltaWait = 30; Limit = 4*18; break;
        case 5: DeltaWait = 27; Limit = 4*20; break;
        case 6: DeltaWait = 20; Limit = 4*27; break;
        case 7: DeltaWait = 18; Limit = 4*30; break;
        case 8: DeltaWait = 12; Limit = 4*45; break;
        }
    }
    if(BulletKind == 2)
    {
        switch(DIFF)
        {
            case 1: DeltaWait = 150; Limit = 16*2; break;
            case 2: DeltaWait = 125; Limit = 16*3; break;
            case 3: DeltaWait = 100; Limit = 16*3; break;
            case 4: DeltaWait =  80; Limit = 16*4; break;
            case 5: DeltaWait =  60; Limit = 16*5; break;
            case 6: DeltaWait =  50; Limit = 16*6; break;
            case 7: DeltaWait =  40; Limit = 16*8; break;
            case 8: DeltaWait =  30; Limit = 16*10; break;
        }
    }
    if(BulletKind == 3)
    {
        switch(DIFF)
        {
            case 1: Limit = 30; break;
            case 2: Limit = 40; break;
            case 3: Limit = 50; break;
            case 4: Limit = 60; break;
        }
    }
}

void PlayerMove(PlayerBox& Player, SDL_Event e, int Mode, int quest[])
{
    if(quest[2] == 2)
    {
        if(SDL_PollEvent(&e) == 0 || e.type != SDL_KEYDOWN) Player.FollowCursor(e);
        return;
    }
    if(Mode == 1) if (e.type == SDL_KEYDOWN)
    switch(e.key.keysym.sym){
        case SDLK_1: Player.Xpath =  4; Player.Ypath =  4; break;
        case SDLK_2: Player.Xpath =  8; Player.Ypath =  8; break;
        case SDLK_3: Player.Xpath = 12; Player.Ypath = 12; break;
        case SDLK_4: if(quest[0] == 2) {Player.Xpath = 16; Player.Ypath = 16;} break;
        case SDLK_5: if(quest[1] == 2) {Player.Xpath = 24; Player.Ypath = 24;} break;
        case SDLK_UP:    Player.moveUp(Player.Ypath); break;
        case SDLK_DOWN:  Player.moveDown(Player.Ypath); break;
        case SDLK_RIGHT: Player.moveRight(Player.Xpath); break;
        case SDLK_LEFT:  Player.moveLeft(Player.Xpath); break;
        case SDLK_w: Player.moveUp(Player.Ypath); break;
        case SDLK_x: Player.moveDown(Player.Ypath); break;
        case SDLK_d: Player.moveRight(Player.Xpath); break;
        case SDLK_a: Player.moveLeft(Player.Xpath); break;
        case SDLK_q: Player.moveUp(Player.Ypath); Player.moveLeft(Player.Xpath); break;
        case SDLK_z: Player.moveDown(Player.Ypath); Player.moveLeft(Player.Xpath); break;
        case SDLK_c: Player.moveRight(Player.Xpath); Player.moveDown(Player.Ypath); break;
        case SDLK_e: Player.moveRight(Player.Xpath);Player.moveUp(Player.Ypath); break;
        default: break;
    }
    if(Mode == 2 || Mode == 3) if (e.type == SDL_KEYDOWN)
    switch(e.key.keysym.sym){
        case SDLK_1: Player.Xpath =  4; Player.Ypath =  4; break;
        case SDLK_2: Player.Xpath =  8; Player.Ypath =  8; break;
        case SDLK_3: Player.Xpath = 12; Player.Ypath = 12; break;
        case SDLK_4: if(quest[0] == 2) {Player.Xpath = 16; Player.Ypath = 16;} break;
        case SDLK_5: if(quest[1] == 2) {Player.Xpath = 24; Player.Ypath = 24;} break;
        case SDLK_UP:    Player.moveUp(Player.Ypath); break;
        case SDLK_DOWN:  Player.moveDown(Player.Ypath); break;
        case SDLK_RIGHT: Player.moveRight(Player.Xpath); break;
        case SDLK_LEFT:  Player.moveLeft(Player.Xpath); break;
        default: break;
    }
}

void UpdateBestStat(int Score, int Wait, int &HighScore, int &BestTime)
{
    if (Score > HighScore)       HighScore = Score;
    if (Wait + 120 > BestTime)   BestTime = Wait + 120;
}

char* CurrentMode(int MODE, int DIFF)
{
    char* p = new char[20];
    if(MODE == 1)
    {
        *(p + 0) = 'P'; *(p + 1) = 'l'; *(p + 2) = 'u'; *(p + 3) = 's'; *(p + 4) = ':'; *(p + 5) = ' ';
        *(p + 6) = 'L'; *(p + 7) = 'e'; *(p + 8) = 'v'; *(p + 9) = 'e'; *(p +10) = 'l'; *(p +11) = ' ';
        *(p +12) = 48 + DIFF;
        *(p +13) = 0;
        return p;
    }
    if(MODE == 2)
    {
        *(p + 0) = 'C'; *(p + 1) = 'i'; *(p + 2) = 'r'; *(p + 3) = 'c'; *(p + 4) = 'l'; *(p + 5) = 'e'; *(p + 6) = ':'; *(p + 7) = ' ';
        *(p + 8) = 'L'; *(p + 9) = 'e'; *(p +10) = 'v'; *(p +11) = 'e'; *(p +12) = 'l'; *(p +13) = ' ';
        *(p +14) = 48 + DIFF;
        *(p +15) = 0;
        return p;
    }
    if(MODE == 0)
    {
        *(p + 0) = 'R'; *(p + 1) = 'a'; *(p + 2) = 'n'; *(p + 3) = 'd'; *(p + 4) = 'o'; *(p + 5) = 'm'; *(p + 6) = ':'; *(p + 7) = ' ';
        switch(DIFF)
        {
            case 1: *(p + 8) = 'E'; *(p + 9) = 'a'; *(p + 10) = 's'; *(p + 11) = 'y'; *(p + 12) = 0; break;
            case 2: *(p + 8) = 'N'; *(p + 9) = 'o'; *(p + 10) = 'r'; *(p + 11) = 'm'; *(p + 12) = 'a'; *(p + 13) = 'l'; *(p + 14) = 0; break;
            case 3: *(p + 8) = 'H'; *(p + 9) = 'a'; *(p + 10) = 'r'; *(p + 11) = 'd'; *(p + 12) = 0; break;
            case 4: *(p + 8) = 'V'; *(p + 9) = 'e'; *(p + 10) = 'r'; *(p + 11) = 'y'; *(p + 12) = ' ';
                    *(p +13) = 'H'; *(p +14) = 'a'; *(p + 15) = 'r'; *(p + 16) = 'd'; *(p + 17) = 0; break;
            default: break;
        }
        return p;
    }
    std::cout << "ERROR loading Current Mode\n";
    return NULL;
}

char* GetCurrentStat(int Score, int Wait)
{
// Time: --'--"--(\n)
// Score: ------...
    int i, j = Score, sec = Wait*5/3, digit = 0;
    char* p = new char[50];
    char t[] = "Time: ";
    for(i=0;i<6;i++) *(p+i) = t[i];

    *(p + 13) = 48 + sec%10; sec/=10;
    *(p + 12) = 48 + sec%10; sec/=10;
    *(p + 10) = 48 + sec%10; sec/=10;
    *(p + 9) = '0' + sec%6;  sec/=6;
    *(p + 7) = '0' + sec%10; sec/=10;
    *(p + 6) = '0' + sec%10; sec/=10;

    for(i=14;i<40;i++) *(p + i) = 32; // ' '
    *(p + 8) = 39; // ''
    *(p + 11) = 34; // ""
    *(p + 40) = 'S';
    *(p + 41) = 'c';
    *(p + 42) = 'o';
    *(p + 43) = 'r';
    *(p + 44) = 'e';
    *(p + 45) = ':';
    *(p + 46) = ' ';

    do{ digit++; j/=10; } while(j>0);
    j = Score;
    for(i=0;i<digit;i++)
    {
        *(p + 46 + digit - i) = '0' + j%10;
        j/=10;
    }
    *(p + 47 + digit) = 0;
    return p;
}

char* GetCurrentBestStat(int Score, int Wait)
{
// Best Time: --'--"--(\n)
// High Score: ------...
    int i, j = Score, sec = Wait*5/3, digit = 0;
    char* p = new char[54];
    char t[] = "Best Time: ";
    for(i=0;i<11;i++) *(p+i) = t[i];

    *(p + 18) = 48 + sec%10; sec/=10;
    *(p + 17) = 48 + sec%10; sec/=10;
    *(p + 16) = 34; // ""
    *(p + 15) = 48 + sec%10; sec/=10;
    *(p + 14) = 48 + sec%6;  sec/=6;
    *(p + 13) = 39; // ''
    *(p + 12) = 48 + sec%10; sec/=10;
    *(p + 11) = 48 + sec%10; sec/=10;

    for(i=19;i<30;i++) *(p + i) = 32; // ' '
    *(p + 30) = 'H';
    *(p + 31) = 'i';
    *(p + 32) = 'g';
    *(p + 33) = 'h';
    *(p + 34) = ' ';
    *(p + 35) = 'S';
    *(p + 36) = 'c';
    *(p + 37) = 'o';
    *(p + 38) = 'r';
    *(p + 39) = 'e';
    *(p + 40) = ':';
    *(p + 41) = ' ';

    do{ digit++; j/=10; } while(j>0);
    j = Score;
    for(i=0;i<digit;i++)
    {
        *(p + 41 + digit - i) = '0' + j%10;
        j/=10;
    }
    *(p + 42 + digit) = 0;
    return p;
}

char* GetScore(int DIFF, int MODE, int COLL, int SCORE)
{
    int i=0, j, K=0, digit = 0;
    char* p = new char[45];
    if(MODE<3)
    {
        *(p+0) = 'L';*(p+1) = 'e';*(p+2) = 'v';*(p+3) = 'e';*(p+4) = 'l';*(p+5) = ' ';
        *(p+6) = DIFF + 48;*(p+7) = '-';*(p+8) = MODE + 48;*(p+9) = ' ';
        if(COLL>=0)
        {
            *(p+10) = 'f';*(p+11) = 'a';*(p+12) = 'i';*(p+13) = 'l';*(p+14) = 'e';
            *(p+15) = 'd';*(p+16) = '.';*(p+17) = '\n';
            K = 18;
        }
        else
        {
            *(p+10) = 'c';*(p+11) = 'o';*(p+12) = 'm';*(p+13) = 'p';*(p+14) = 'l';
            *(p+15) = 'e';*(p+16) = 't';*(p+17) = 'e';*(p+18) = 'd';*(p+19) = '.';
            *(p+20) = '\n';
            K = 21;
        }
    }
    if(MODE==3)
    {
        if(COLL<=0)
        {
            *(p+0) = 'Y';*(p+1) = 'o';*(p+2) = 'u';*(p+3) = ' ';*(p+4) = 'q';*(p+5) = 'u';
            *(p+6) = 'i';*(p+7) = 't';*(p+8) = 'e';*(p+9) = 'd';*(p+10) = '!';*(p+11) = '\n';
            K=12;
;       }
        else if(SCORE>99999)
        {
            *(p+0) = 'Y';*(p+1) = 'o';*(p+2) = 'u';*(p+3) = ' ';*(p+4) = 'a';*(p+5) = 'r';
            *(p+6) = 'e';*(p+7) = ' ';*(p+8) = 'v';*(p+9) = 'e';*(p+10) = 'r';*(p+11) = 'y';
            *(p+12) = ' ';*(p+13) = 'g';*(p+14) = 'o';*(p+15) = 'o';*(p+16) = 'd';*(p+17) = '!';
            *(p+18) = '\n'; K = 19;
        }
        else
        {
            *(p+0) = 'W';*(p+1) = 'e';*(p+2) = 'l';*(p+3) = 'l';*(p+4) = ' ';*(p+5) = 'd';
            *(p+6) = 'o';*(p+7) = 'n';*(p+8) = 'e';*(p+9) = '!';*(p+10) = '\n';
            K=11;
        }
    }
    *(p+K) = 'Y';  *(p+K+1) = 'o';*(p+K+2) = 'u';*(p+K+3) = 'r';*(p+K+3) = ' ';
    *(p+K+4) = 'S';*(p+K+5) = 'c';*(p+K+6) = 'o';*(p+K+7) = 'r';*(p+K+8) = 'e';*(p+K+9) = ':';*(p+K+10) = ' ';

    j = SCORE;
    do{ digit++; j/=10; } while(j>0);
    j = SCORE;
    for(i=0;i<digit;i++)
    {
        *(p + K + 10 + digit - i) = '0' + j%10;
        j/=10;
    }
    *(p+K+11+digit) = '.';
    *(p+K+12+digit) = 0;
    return p;
}

char* GetQuestStatus(int QuestNum)
{
    char* p = new char[8]; int k;
    char Given[3][8] = {"Locked\0", "Off\0", "On\0"};
    for(k = 0; Given[QuestNum][k] > 0; k++) *(p+k) = Given[QuestNum][k];
    *(p+k) = 0;
    return p;
}

char* GetQuest(int QuestNum)
{
    char* p = new char[400];
    int k;
    char Given[3][400] = {
        "To unlock '4' key, you have to complete 2 of these tasks: \n"
        "1.  In Plus Level 3 or higher: Score 15000 points.\n"
        "2.  In Circle Level 3 or higher: Survive in 1 minute.\n"
        "3.  In Random (Normal or harder): Score 600 points.\0",

        "To unlock '5' key, you have to complete 2 of these tasks: \n"
        "1.  In Plus Level 4 or higher: Score 15000 points.\n"
        "2.  In Circle Level 4 or higher: Survive in 1 minute.\n"
        "3.  In Random (Normal or harder): Score 600 points.\0",

        "To unlock mouse control, you have to complete 2 of these tasks: \n"
        " 1. In Plus Level 5 or higher: Score 15000 points.\n"
        " 2. In Circle Level 5 or higher: Survive in 1 minute.\n"
        " 3. In Random (Hard or Very Hard): Score 600 points.\n"
        "WARNING: If you use mouse to control the player, "
        "try not to move the cursor too much while a message box is being shown.\0",
    };
    for(k = 0; Given[QuestNum][k] > 0; k++) *(p+k) = Given[QuestNum][k];
    *(p+k) = 0;
    return p;
}
