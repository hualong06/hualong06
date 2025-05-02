#ifndef MAP_H
#define MAP_H

#include "commonfunc.h"

struct player;

struct tileMap {
    int tileSize;
    int mapWidth;
    int mapHeight;
    int tileMapWidth;
    int tileMapHeight;
    int **gameMap;
    string filename;
    SDL_Texture* texture;
};

enum RoomType {
    BEDROOM,    // Phòng ngủ
    LIVINGROOM, // Phòng khách
    BATHROOM,   // Phòng tắm
    HALLWAY     // Hành lang
};

void initTileMap(tileMap &map, SDL_Renderer* &renderer);
bool loadMap(tileMap &map);
void renderMap(SDL_Renderer* &renderer, tileMap &map);

#endif