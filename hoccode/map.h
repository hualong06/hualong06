#ifndef MAP_H
#define MAP_H

#include "commonfunc.h"

struct player;

struct tileMap {
    int tileSize;
    int mapWidth;
    int mapHeight;
    int **gameMap;
    string filename;
    SDL_Texture* texture;
};

void initTileMap(tileMap &map, SDL_Renderer* &renderer);
bool loadMap(tileMap &map);
void renderMap(SDL_Renderer* &renderer,SDL_Rect &camera, tileMap &map, player &p);
void updateCamera(SDL_Rect &camera, player &p, tileMap &map);

#endif