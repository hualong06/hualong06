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
    int direction; // 0: xuống, 1: phải, 2: trái, 3: lên, 4-7: chéo
    int currentFrame;
    int frameCount;
    int frameWidth, frameHeight;
    int frameDiagonalWidth, frameDiagonalHeight;
    SDL_Texture* texture;          // Texture cho di chuyển thẳng
    SDL_Texture* diagonalTexture;  // Texture cho di chuyển chéo
};

void initPlayer(player &p, SDL_Renderer* &renderer);
void handleEvents(bool &running, player &p, int &new_x, int &new_y);
void update(player &p, int &new_x, int &new_y, tileMap &map, warning &warning_);
void renderPlayer(SDL_Renderer* &renderer, player &p);

#endif