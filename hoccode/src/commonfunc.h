#ifndef COMMONFUNC_H
#define COMMONFUNC_H

#define SDL_MAIN_HANDLED
#include <iostream>
#include <string>
#include <time.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <SDL2\SDL.h>
#include <SDL2\SDL_image.h>
#include <SDL2\SDL_ttf.h>

using namespace std;

const int SCREEN_WIDTH = 16*64;
const int SCREEN_HEIGHT = 16*36;

enum GameState {
    MENU,
    PLAYING,
    SELECT_HOUSE,
    HOUSE_MAP,
    ENDGAME,
    EXIT
};


#endif