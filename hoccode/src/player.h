#ifndef PLAYER_H
#define PLAYER_H

#include "commonfunc.h"

struct tileMap;
struct base_object;
struct warning;

struct player {
    int x, y;
    int width, height;
    int speed;
    int direction; 
    int currentFrame;
    int frameCount;
    int frameWidth, frameHeight;
    int frameDiagonalWidth, frameDiagonalHeight;
    SDL_Texture* texture;          
    SDL_Texture* diagonalTexture;  
};

void initPlayer(player &p, SDL_Renderer* &renderer);
void handleEvents(bool &running, player &p, int &new_x, int &new_y);
void update(player &p, int &new_x, int &new_y, tileMap &map, object &warning);
void renderPlayer(SDL_Renderer* &renderer, player &p);

#endif