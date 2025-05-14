#ifndef PLAYER_H
#define PLAYER_H

#include "commonfunc.h"

struct tileMap;
struct object;
struct icon;

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
void handleMovement(player &p, int &new_x, int &new_y);
bool isWalkableByMap(int x, int y, int w, int h, tileMap &map);
void update(player &p, int &new_x, int &new_y, tileMap &map);
void renderPlayer(SDL_Renderer* &renderer, player &p);
void resetGame(SDL_Renderer* &renderer, player &p, object &warning, icon &gold, tileMap &map, GameState &state);
#endif