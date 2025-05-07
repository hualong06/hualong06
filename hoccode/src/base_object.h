#ifndef BASE_OBJECT_H
#define BASE_OBJECT_H

#include "commonfunc.h"

struct tileMap;
struct player;

struct object {
    int x, y;
    int width, height;
    int frameWidth;
    int frameHeight;
    int direction;
    SDL_Texture* texture;
};

struct icon {
    int x, y;
    int width, height;
    int frameWidth;
    int frameHeight;
    int currentFrame;
    int count;
    SDL_Texture* texture;
};

enum GameState {
    MENU,
    PLAYING,
    SELECT_HOUSE,
    HOUSE_MAP,
    EXIT
};

struct menu{
    SDL_Texture* textrueMap;
    SDL_Texture* textrueMenu;
    TTF_Font* font;
};

void initObject(object &safe, SDL_Renderer* &renderer);
void initwarning(object &warning);
void initIcon(icon &icon_, SDL_Renderer* &renderer);
void initMenu(menu &Menu, SDL_Renderer* &renderer);
void initGold(icon &gold, SDL_Renderer* &renderer);

void renderWarning(SDL_Renderer* &renderer, object &warning);
void renderMenu(SDL_Renderer* &renderer, menu &Menu);
void handleMenuEvents(bool &running, GameState &state);
void renderGold(SDL_Renderer* &renderer, icon &gold, menu &Menu);
void handleSelectHouseEvents(bool &running, GameState &state);
void updateGold(SDL_Renderer* &renderer, player &p, tileMap &map, icon &gold);

#endif