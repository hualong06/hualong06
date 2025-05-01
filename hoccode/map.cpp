#include "commonfunc.h"
#include "map.h"
#include "player.h"

void initTileMap(tileMap &map, SDL_Renderer* &renderer) {
    map.tileSize = 16;
    map.mapWidth = 16*64;
    map.mapHeight = 16*36;
    map.tileMapWidth = 64;
    map.tileMapHeight = 36;
    map.texture = IMG_LoadTexture(renderer, "LIVIROOM.png");
    if(map.texture == NULL) {
        cerr << "Failed to load map texture SDL_Error:" << SDL_GetError() << endl;
    }
    map.gameMap = new int*[map.tileMapHeight];
    for(int i=0; i<map.tileMapHeight; i++){
        map.gameMap[i] = new int[map.tileMapWidth];
    }
    map.filename = "mapgame.csv";
}

bool loadMap(tileMap &map){
    ifstream file(map.filename);
    if(!file){
        cerr << "Failed to load map file!" << endl;
        return false;
    }

    string line;
    for(int i = 0; i < map.tileMapHeight; i++) {
        if(!getline(file, line)) {
            cerr << "Error: Not enough rows in the map file!" << endl;
            return false;
        }

        stringstream ss(line);
        string value;
        for(int j = 0; j < map.tileMapWidth; j++) {
            if(!getline(ss, value, ',')) {
                cerr << "Error: Not enough columns in the map file!" << endl;
                return false;
            }
            map.gameMap[i][j] = stoi(value);
        }
    }

    file.close();
    return true;
}

void renderMap(SDL_Renderer* &renderer, tileMap &map){
    SDL_Rect liviroomRect = { 0, 0, map.mapWidth, map.mapHeight };

    if(map.texture != NULL) {
        SDL_RenderCopy(renderer, map.texture, NULL, &liviroomRect);
    }
}

void renderLiviroomObj(SDL_Renderer* &renderer, tileMap &map){
    SDL_Rect liviroomObjRect = {};

    if(map.texture) ;   
    
}

