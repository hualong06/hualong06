#include "commonfunc.h"
#include "map.h"
#include "player.h"


void initTileMap(tileMap &map, SDL_Renderer* &renderer) {
    map.tileSize = 16;
    map.mapWidth = 16*120;
    map.mapHeight = 16*67;
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

void updateCamera(SDL_Rect &camera, player &p, tileMap &map) {
    camera.x = p.x - SCREEN_WIDTH / 2;
    camera.y = p.y - SCREEN_HEIGHT / 2;
    camera.w = SCREEN_WIDTH;
    camera.h = SCREEN_HEIGHT;

    if (camera.x < 0) camera.x = 0;
    if (camera.y < 0) camera.y = 0;
    if (camera.x + camera.w > map.mapWidth) camera.x = map.mapWidth - camera.w;
    if (camera.y + camera.h > map.mapHeight) camera.y = map.mapHeight - camera.h;
}

void renderMap(SDL_Renderer* &renderer, SDL_Rect &camera, tileMap &map, player &p){

    SDL_Rect mapRect = { 0, 0, map.mapWidth, map.mapHeight };

    if(map.texture != NULL) {
        SDL_RenderCopy(renderer, map.texture, &camera, NULL);
    }
}

