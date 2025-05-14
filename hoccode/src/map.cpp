#include "commonfunc.h"
#include "map.h"
#include "player.h"
#include "base_object.h"
#include "gameflow.h"

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

    map.filename = "liviroom.csv";
    map.fileNameGold = "liviroomGold.csv";
}

bool loadMap(tileMap &map) {
    map.gameMap = new int*[map.tileMapHeight];
    for(int i=0; i<map.tileMapHeight; i++){
        map.gameMap[i] = new int[map.tileMapWidth];
    }

    map.mapGold = new int*[9];
    for(int i = 0; i < 9; i++){
        map.mapGold[i] = new int[16];
    }

    ifstream file(map.filename);
    if (!file) {
        cerr << "Failed to load map file: " << map.filename << endl;
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

    ifstream goldFile(map.fileNameGold);
    if (!goldFile) {
        cerr << "Failed to load gold map file: " << map.fileNameGold << endl;
        return false;
    }

    for (int i = 0; i < 9; i++) {
        if (!getline(goldFile, line)) {
            cerr << "Error: Not enough rows in the gold map file!" << endl;
            return false;
        }

        stringstream ss(line);
        string value;
        for (int j = 0; j < 16; j++) {
            if (!getline(ss, value, ',')) {
                cerr << "Error: Not enough columns in the gold map file!" << endl;
                return false;
            }
            map.mapGold[i][j] = stoi(value);
        }
    }
    goldFile.close();

    return true;
}

void renderMap(SDL_Renderer* &renderer,tileMap &map, SDL_Texture* texture) {
    SDL_Rect liviroomRect = { 0, 0, map.mapWidth, map.mapHeight };

    if(texture != NULL) {
        SDL_RenderCopy(renderer, texture, NULL, &liviroomRect);
    }
}

void changeRoom(player &p, tileMap &map, string &newMapFile, string &newMapGold, int startX, int startY) {
    for(int i = 0; i < map.tileMapHeight; i++) {
        delete[] map.gameMap[i];
    }
    delete[] map.gameMap;

    map.filename = newMapFile;
    map.fileNameGold = newMapGold;

    if(!loadMap(map)) {
        cerr << "Failed to load new map: "  << newMapGold << endl;
        return;
    }

    for(int i=0; i< map.tileMapHeight; i++) {
        for(int j=0; j< map.tileMapWidth; j++) {
            cout<<map.gameMap[i][j]<<", ";
        }
        cout << endl;
    }

    p.x = startX;
    p.y = startY;
}

void checkAndChangeRoom(SDL_Renderer* &renderer ,player &p, tileMap &map, const SDL_Event &event) {
    int tileX = (p.x + p.width / 2 -8) / map.tileSize;
    int tileY = (p.y + p.height -16) / map.tileSize;

    string newMapRoom;
    string newMapGold;

    if(map.gameMap[tileY][tileX] == 5 && checkInteraction(event)) {
        newMapRoom = "bathroom.csv";
        newMapGold = "mapGoldBedroom.csv";
        changeRoom(p, map, newMapRoom, newMapGold, 24 * 16, 23 * 16); 

        map.textureLayer1 = IMG_LoadTexture(renderer, "images/bathroom.png");
        if(map.textureLayer1 == NULL) {
            cerr << "Failed to load bathroom map texture SDL_Error:" << SDL_GetError() << endl;
        }

        map.textureLayer2 = NULL;
        map.textureLayer3 = NULL;
        map.textureLayer4 = NULL;

    } else if((map.gameMap[tileY][tileX] == 6 || map.gameMap[tileY][tileX] == 8) && checkInteraction(event)) {
        newMapRoom = "hallway.csv";
        newMapGold = "hallwayGold.csv";

        if(map.gameMap[tileY][tileX] == 6) {
            changeRoom(p, map, newMapRoom, newMapGold, 16 * 16, 22 * 16);
        } 
        else {
            changeRoom(p, map, newMapRoom, newMapGold, 39 * 16, 25 * 16);
        }

        map.textureLayer1 = IMG_LoadTexture(renderer, "images/hallway.png");
        if(map.textureLayer1 == NULL) {
            cerr << "Failed to load hallway map texture SDL_Error:" << SDL_GetError() << endl;
        }

        map.textureLayer2 = NULL;
        map.textureLayer3 = NULL;
        map.textureLayer4 = NULL;

    } else if(map.gameMap[tileY][tileX] == 7 && checkInteraction(event)) {
        newMapRoom = "bedroom.csv";
        newMapGold = "mapGoldBedroom.csv";
        changeRoom(p, map, newMapRoom, newMapGold, 33 * 16, 27 * 16); 

        map.textureLayer1 = IMG_LoadTexture(renderer, "images/bedroomLayer1.png");
        if(map.textureLayer1 == NULL) {
            cerr << "Failed to load bedroomLayer1 map texture SDL_Error:" << SDL_GetError() << endl;
        } 
        map.textureLayer2 = IMG_LoadTexture(renderer, "images/bedroomLayer2.png");
        if(map.textureLayer2 == NULL) {
            cerr << "Failed to load bedroomLayer2 map texture SDL_Error:" << SDL_GetError() << endl;
        }

        map.textureLayer3 = NULL;
        map.textureLayer4 = NULL;

    } else if((map.gameMap[tileY][tileX] == 9 || map.gameMap[tileY][tileX] == 10) && checkInteraction(event)) {
        newMapRoom = "liviroom.csv";
        newMapGold = "liviroomGold.csv";

        if(map.gameMap[tileY][tileX] == 9) {
            changeRoom(p, map, newMapRoom, newMapGold, 19 * 16, 17 * 16);
        } else {
            changeRoom(p, map, newMapRoom, newMapGold, 7 * 16, 15 * 16);   
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

void render(SDL_Renderer* &renderer, player &p, tileMap &map) {
    int tileX = (p.x + p.width / 2 -8) / map.tileSize;
    int tileY = (p.y + p.height - 1) / map.tileSize;
    
    if(map.gameMap[tileY][tileX] == 1 
        || map.gameMap[tileY][tileX] == 5 
        || map.gameMap[tileY][tileX] == 8 
        || map.gameMap[tileY][tileX] == 9 
        || map.gameMap[tileY][tileX] ==10) {
        renderMap(renderer, map, map.textureLayer1);
        renderMap(renderer, map, map.textureLayer2);
        renderMap(renderer, map, map.textureLayer3);
        renderMap(renderer, map, map.textureLayer4);
        renderPlayer(renderer, p);
    } else if(map.gameMap[tileY][tileX] == 2 || map.gameMap[tileY][tileX] == 12) {
        renderMap(renderer, map, map.textureLayer1);
        renderMap(renderer, map, map.textureLayer3);
        renderMap(renderer, map, map.textureLayer4);
        renderPlayer(renderer, p);
        renderMap(renderer, map, map.textureLayer2);
    } else if(map.gameMap[tileY][tileX] == 4) {
        renderMap(renderer, map, map.textureLayer1);
        renderMap(renderer, map, map.textureLayer4);
        renderMap(renderer, map, map.textureLayer2);
        renderPlayer(renderer, p);
        renderMap(renderer, map, map.textureLayer3);
    } else if(map.gameMap[tileY][tileX] == 3 || map.gameMap[tileY][tileX] == 6) {
        renderMap(renderer, map, map.textureLayer1);
        renderMap(renderer, map, map.textureLayer2);
        renderMap(renderer, map, map.textureLayer3);
        renderPlayer(renderer, p);
        renderMap(renderer, map, map.textureLayer4);
    }
}

