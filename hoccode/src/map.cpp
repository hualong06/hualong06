#include "commonfunc.h"
#include "map.h"
#include "player.h"

void initTileMap(tileMap &map, SDL_Renderer* &renderer) {

    map.tileSize = 16;
    map.mapWidth = 16*64;
    map.mapHeight = 16*36;
    map.tileMapWidth = 64;
    map.tileMapHeight = 36;

    map.textureLayer1 = IMG_LoadTexture(renderer, "images/liviroomLayer1.png");
    if(map.textureLayer1 == NULL) {
        cerr << "Failed to load map texture SDL_Error:" << SDL_GetError() << endl;
    }
    map.textureLayer2 = IMG_LoadTexture(renderer, "images/liviroomLayer2.png");
    if(map.textureLayer2 == NULL) {
        cerr << "Failed to load map texture SDL_Error:" << SDL_GetError() << endl;
    } 
    map.textureLayer3 = IMG_LoadTexture(renderer, "images/liviroomLayer3.png");
    if(map.textureLayer3 == NULL) {
        cerr << "Failed to load map texture SDL_Error:" << SDL_GetError() << endl;
    } 
    map.textureLayer4 = IMG_LoadTexture(renderer, "images/liviroomLayer4.png");
    if(map.textureLayer4 == NULL) {
        cerr << "Failed to load map texture SDL_Error:" << SDL_GetError() << endl;
    }

    map.gameMap = new int*[map.tileMapHeight];
    for(int i=0; i<map.tileMapHeight; i++){
        map.gameMap[i] = new int[map.tileMapWidth];
    }

    map.filename = "maptile.csv";
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

void renderMap(SDL_Renderer* &renderer,tileMap &map, SDL_Texture* texture) {
    
    SDL_Rect liviroomRect = { 0, 0, map.mapWidth, map.mapHeight };

    if(texture != NULL) {
        SDL_RenderCopy(renderer, texture, NULL, &liviroomRect);
    }
}

void changeRoom(player &p, tileMap &map, string &newMapFile, int startX, int startY) {

    for(int i = 0; i < map.tileMapHeight; i++) {
        delete[] map.gameMap[i];
    }
    delete[] map.gameMap;

    map.filename = newMapFile;

    if(!loadMap(map)) {
        cerr << "Failed to load new map: "  << newMapFile << endl;
        return;
    }

    p.x = startX;
    p.y = startY;
}

void checkAndChangeRoom(SDL_Renderer* &renderer ,player &p, tileMap &map) {

    int tileX = p.x / map.tileSize;
    int tileY = p.y / map.tileSize;
    string newMapRoom;

    if(map.gameMap[tileY][tileX] == 2) {

        newMapRoom = "bathroom.csv";
        changeRoom(p, map, newMapRoom, 64, 64); 

        map.textureLayer1 = IMG_LoadTexture(renderer, "images/bathroom.png");
        if(map.textureLayer1 == NULL) {
            cerr << "Failed to load bathroom map texture SDL_Error:" << SDL_GetError() << endl;
        }
    } else if(map.gameMap[tileY][tileX] == 3) {

        newMapRoom = "hallway.csv";
        changeRoom(p, map, newMapRoom, 64, 64);

        map.textureLayer1 = IMG_LoadTexture(renderer, "images/hallway.png");
        if(map.textureLayer1 == NULL) {
            cerr << "Failed to load hallway map texture SDL_Error:" << SDL_GetError() << endl;
        }

    } else if(map.gameMap[tileY][tileX] == 4) {

        newMapRoom = "bedroom.csv";
        changeRoom(p, map, newMapRoom, 64, 64); 

        map.textureLayer1 = IMG_LoadTexture(renderer, "images/bedroomLayer1.png");
        if(map.textureLayer1 == NULL) {
            cerr << "Failed to load bedroomLayer1 map texture SDL_Error:" << SDL_GetError() << endl;
        } 
        map.textureLayer2 = IMG_LoadTexture(renderer, "images/bedroomLayer2.png");
        if(map.textureLayer2 == NULL) {
            cerr << "Failed to load bedroomLayer2 map texture SDL_Error:" << SDL_GetError() << endl;
        }

    } else if(map.gameMap[tileY][tileX] == 5 || map.gameMap[tileY][tileX] == 6) {

        newMapRoom = "liviroom.csv";

        if(map.gameMap[tileY][tileX] == 5) {
            changeRoom(p, map, newMapRoom, 9, 9);
        } else {
            changeRoom(p, map, newMapRoom, 9, 9);   
        }

        map.textureLayer1 = IMG_LoadTexture(renderer, "images/liviroomLayer1.png");
        if(map.textureLayer1 == NULL) {
            cerr << "Failed to load liviroomLayer1 map texture SDL_Error:" << SDL_GetError() << endl;
        }
        map.textureLayer2 = IMG_LoadTexture(renderer, "images/liviroomLayer2.png");
        if(map.textureLayer2 == NULL) {
            cerr << "Failed to load liviroomLayer2 map texture SDL_Error:" << SDL_GetError() << endl;
        } 
        map.textureLayer3 = IMG_LoadTexture(renderer, "images/liviroomLayer3.png");
        if(map.textureLayer3 == NULL) {
            cerr << "Failed to load liviroomLayer3 map texture SDL_Error:" << SDL_GetError() << endl;
        } 
        map.textureLayer4 = IMG_LoadTexture(renderer, "images/liviroomLayer4.png");
        if(map.textureLayer4 == NULL) {
            cerr << "Failed to load liviroomLayer4 map texture SDL_Error:" << SDL_GetError() << endl;
        }

    }
}