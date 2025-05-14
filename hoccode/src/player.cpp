#include "commonfunc.h"
#include "player.h"
#include "base_object.h"
#include "map.h"
#include "gameflow.h"

void initPlayer(player &p, SDL_Renderer* &renderer) {
    p.x = 23 * 16;
    p.y = 25 * 16;
    p.width = 93 * 7 / 8;
    p.height = 96 * 7 / 8;
    p.speed = 4;

    p.texture = IMG_LoadTexture(renderer, "images/thief.png");
    if (p.texture == NULL) {
        cerr << "Failed to load player texture! SDL_Error: " << SDL_GetError() << endl;
    }

    p.diagonalTexture = IMG_LoadTexture(renderer, "images/thief_diagonal.png");
    if (p.diagonalTexture == NULL) {
        cerr << "Failed to load diagonal texture! SDL_Error: " << SDL_GetError() << endl;
    }

    p.currentFrame = 0;
    p.frameCount = 4;
    p.frameWidth = 372;
    p.frameHeight = 384;
    p.frameDiagonalWidth = 1024 / 4;
    p.frameDiagonalHeight = 1024 / 4;
}

void handleMovement(player &p, int &new_x, int &new_y) {
    
    new_x = p.x;
    new_y = p.y;

    const Uint8* keystate = SDL_GetKeyboardState(NULL);

    if(keystate[SDL_SCANCODE_LSHIFT]) {
        p.speed = 8;
    } else {
        p.speed = 4;
    }

    if (keystate[SDL_SCANCODE_UP] && keystate[SDL_SCANCODE_LEFT]) {
        new_y -= p.speed * 0.5;
        new_x -= p.speed;
        p.direction = 5; 
    } else if (keystate[SDL_SCANCODE_UP] && keystate[SDL_SCANCODE_RIGHT]) {
        new_y -= p.speed * 0.5;
        new_x += p.speed;
        p.direction = 7; 
    } else if (keystate[SDL_SCANCODE_DOWN] && keystate[SDL_SCANCODE_LEFT]) {
        new_y += p.speed * 0.5;
        new_x -= p.speed;
        p.direction = 6; 
    } else if (keystate[SDL_SCANCODE_DOWN] && keystate[SDL_SCANCODE_RIGHT]) {
        new_y += p.speed * 0.5;
        new_x += p.speed;
        p.direction = 4; 
    } else if (keystate[SDL_SCANCODE_UP]) {
        new_y -= p.speed;
        p.direction = 3; 
    } else if (keystate[SDL_SCANCODE_DOWN]) {
        new_y += p.speed;
        p.direction = 0; 
    } else if (keystate[SDL_SCANCODE_LEFT]) {
        new_x -= p.speed;
        p.direction = 2; 
    } else if (keystate[SDL_SCANCODE_RIGHT]) {
        new_x += p.speed;
        p.direction = 1; 
    }

    if (keystate[SDL_SCANCODE_UP] || keystate[SDL_SCANCODE_DOWN] || 
        keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_RIGHT]) {
        p.currentFrame = (SDL_GetTicks() / 100) % p.frameCount;
    }
}

bool isWalkableByMap(int x, int y, int w, int h, tileMap &map) {
    int footLeftX = x;
    int footRightX = x + w - 1;
    int footY = y + h - 1;
    int headY = y + h - 20; 

    int tileLeftX = footLeftX / map.tileSize;
    int tileRightX = footRightX / map.tileSize;
    int tileFootY = footY / map.tileSize;
    int tileHeadY = headY / map.tileSize;

    if (map.gameMap[tileFootY][tileLeftX] == 0 || map.gameMap[tileFootY][tileRightX] == 0) {
        return false; 
    }

    if (map.gameMap[tileHeadY][tileLeftX] == 0 || map.gameMap[tileHeadY][tileRightX] == 0) {
        return false;
    }

    return true;
}


void update(player &p, int &new_x, int &new_y, tileMap &map) {

    if (isWalkableByMap(new_x, new_y, p.width, p.height, map)) {

        p.x = new_x;
        p.y = new_y;
    }

    int tileX = (p.x + p.width / 2 -8) / map.tileSize;
    int tileY = (p.y + p.height -16) / map.tileSize;
    int tileGoldX = (p.x + p.width / 2 ) / 64;
    int tileGoldY = (p.y + p.height ) / 64;

    cout << p.x <<" " << p.y << " " <<tileX <<" "<<tileY <<" "<<map.gameMap[tileY][tileX] << " " << tileGoldX <<" " << tileGoldY << " " <<map.mapGold[tileGoldY][tileGoldX] << endl;

}

void renderPlayer(SDL_Renderer* &renderer, player &p) {
    SDL_Rect playerRect = { p.x, p.y, p.width, p.height };

    if (p.direction >= 4 && p.direction <= 7) {
        SDL_Rect frameDiagonalRect;
        frameDiagonalRect.w = p.frameDiagonalWidth;
        frameDiagonalRect.h = p.frameDiagonalHeight;
        frameDiagonalRect.x = p.currentFrame * p.frameDiagonalWidth;
        frameDiagonalRect.y = (p.direction - 4) * p.frameDiagonalHeight;

        SDL_RenderCopy(renderer, p.diagonalTexture, &frameDiagonalRect, &playerRect);
    } else {
        SDL_Rect frameRect;
        frameRect.w = p.frameWidth;
        frameRect.h = p.frameHeight;
        frameRect.x = p.currentFrame * p.frameWidth;
        frameRect.y = p.direction * p.frameHeight;
        
        SDL_RenderCopy(renderer, p.texture, &frameRect, &playerRect);
    }
}

void resetGame(SDL_Renderer* &renderer, player &p, object &warning, icon &gold, tileMap &map, GameState &state) {

    p.x = 23 * 16;
    p.y = 25 * 16;

    warning.direction = 0;

    gold.count = 0;

    map.filename = "liviroom.csv";
    map.fileNameGold = "mapGoldLiviroom.csv";
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

    state = PLAYING;
}