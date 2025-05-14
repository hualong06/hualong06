#ifndef BASE_OBJECT_H
#define BASE_OBJECT_H

#include "commonfunc.h"

struct tileMap;
struct player;
struct menu;

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

void initObject(object &safe, SDL_Renderer* &renderer);
void initwarning(object &warning);
void initIcon(icon &icon_, SDL_Renderer* &renderer);
void initGold(icon &gold, SDL_Renderer* &renderer);

void renderWarning(SDL_Renderer* &renderer, object &warning);
void renderGold(SDL_Renderer* &renderer, icon &gold, menu &Menu);
void updateGold(SDL_Renderer* &renderer, player &p, tileMap &map, icon &gold, SDL_Event &event);
bool checkInteraction(const SDL_Event& event);
void checkEndGame(player &p, tileMap &map, object &warning, GameState &state, string &status);
void renderGoldProgressBar(SDL_Renderer* &renderer, icon &gold, int totalGold);

#endif