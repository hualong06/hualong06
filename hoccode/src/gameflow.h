#ifndef GAMEFLOW_
#define GAMEFLOW_

#include "commonfunc.h"

struct tileMap;
struct player;
struct tileMap;
struct icon;


struct menu{
    SDL_Texture* textrueMap;
    SDL_Texture* textrueMenu;
    TTF_Font* font;
};

void initMenu(menu &Menu, SDL_Renderer* &renderer);
void renderMenu(SDL_Renderer* &renderer, menu &Menu);
void handleEndGameEvents(SDL_Renderer* renderer, bool& running, GameState& state, player& p, tileMap& map, icon& gold, menu& Menu, object &warning);
void fadeIn(SDL_Renderer* renderer, SDL_Texture* darkTexture, bool& running, icon& gold, menu& Menu, string status);
SDL_Texture* createDarkTexture(SDL_Renderer* renderer);
void renderEndGameMenu(SDL_Renderer* renderer, string status, icon gold, menu Menu);
void handleMenuEvents(SDL_Renderer* renderer, bool& running, GameState& state);


#endif