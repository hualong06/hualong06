#include "commonfunc.h"
#include "map.h"
#include "player.h"


void initTileMap(tileMap &map, SDL_Renderer* &renderer) {
    map.tileSize = 16;
    map.mapWidth = 16*120*3/2;
    map.mapHeight = 16*67*3/2;
    map.texture = IMG_LoadTexture(renderer, "home.png");
    if(map.texture == NULL) {
        cerr << "Failed to load map texture SDL_Error:" << SDL_GetError() << endl;
    }
    map.gameMap = new int*[map.mapHeight/map.tileSize];
    for(int i=0; i<map.mapHeight/map.tileSize; i++){
        map.gameMap[i] = new int[map.mapWidth/map.tileSize];
    }
    map.filename = "maptile.csv";
}

bool loadMap(tileMap &map){
    ifstream file(map.filename);
    if(!file){
        cerr<<"Failed to load map file!"<<endl;
        return false;
    }
    for(int i=0; i<map.mapHeight/map.tileSize; i++){
        for(int j=0; j<map.mapWidth/map.tileSize; j++){
            file >> map.gameMap[i][j];
        }
    }
    file.close();
    return true;
}

void renderMap(SDL_Renderer* &renderer, tileMap &map){
    
}