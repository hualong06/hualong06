#ifndef MAP_H
#define MAP_H

#include "commonfunc.h"

struct player;
struct icon;

struct tileMap {
    int tileSize;
    int mapWidth;
    int mapHeight;
    int tileMapWidth;
    int tileMapHeight;
    int **gameMap;
    int **mapGold;
    string filename;
    string fileNameGold;
    SDL_Texture* textureLayer1;
    SDL_Texture* textureLayer2;
    SDL_Texture* textureLayer3;
    SDL_Texture* textureLayer4;
};

void initTileMap(tileMap &map, SDL_Renderer* &renderer);
bool loadMap(tileMap &map);
void changeRoom(player &p, tileMap &map, string &newMapFile,  string &newMapGold, int startX, int startY);
void checkAndChangeRoom(SDL_Renderer* &renderer, player &p, tileMap &map, const SDL_Event &event);
void renderMap(SDL_Renderer* &renderer, tileMap &map, SDL_Texture* texture);
void render(SDL_Renderer* &renderer, player &p, tileMap &map);


#endif