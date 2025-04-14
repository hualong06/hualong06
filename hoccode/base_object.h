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
    SDL_Texture* texture;
};

struct warning {
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
    int frameCount;
    SDL_Texture* texture;
};

enum GameState {
    MENU,
    SELECT_HOUSE,
    HOUSE_MAP,
    EXIT
};

void initObject(object &safe, SDL_Renderer* &renderer);
void initwarning(warning &warning_);
void initIcon(icon &icon_, SDL_Renderer* &renderer);
void renderWarning(SDL_Renderer* &renderer, warning &warning_);

void renderMenu(SDL_Renderer* &renderer, TTF_Font* &font);
void handleMenuEvents(bool &running, GameState &state);
void renderSelectHouse(SDL_Renderer* &renderer, TTF_Font* &font);
void handleSelectHouseEvents(bool &running, GameState &state);

#endif