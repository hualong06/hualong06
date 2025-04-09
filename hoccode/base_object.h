#ifndef BASE_OBJECT_H
#define BASE_OBJECT_H

#include "commonfunc.h"

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

void initObject(object &safe, SDL_Renderer* &renderer);
void initwarning(warning &warning_);
void initIcon(icon &icon_, SDL_Renderer* &renderer);



#endif