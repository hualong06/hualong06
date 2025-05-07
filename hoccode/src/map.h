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
    int **map;
    string filename;
    SDL_Texture* textureLayer1;
    SDL_Texture* textureLayer2;
    SDL_Texture* textureLayer3;
    SDL_Texture* textureLayer4;
    roomType currentRoom;
};

enum roomType {
    BEDROOM,    // Phòng ngủ
    LIVINGROOM, // Phòng khách
    BATHROOM,   // Phòng tắm
    HALLWAY     // Hành lang
};

void initTileMap(tileMap &map, SDL_Renderer* &renderer);
bool loadMap(tileMap &map);
void changeRoom(player &p, tileMap &map, string &newMapFile, int startX, int startY);
void checkAndChangeRoom(player &p, tileMap &map);
void renderMap(SDL_Renderer* &renderer, tileMap &map, SDL_Texture* texture);


#endif