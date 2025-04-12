#include "commonfunc.h"
#include "player.h"
#include "base_object.h"
#include "map.h"

void initPlayer(player &p, SDL_Renderer* &renderer) {
    p.x = 58*16;
    p.y = 49*16;
    p.width = 93*4/3;
    p.height = 96*4/3;
    p.speed = 30;
    p.texture = IMG_LoadTexture(renderer, "thief.png");
    if (p.texture == NULL) {
        cerr << "Failed to load player texture! SDL_Error: " << SDL_GetError() << endl;
    }
    p.currentFrame = 0;
    p.frameCount = 4;
    p.frameWidth = 372;
    p.frameHeight = 384;
}

void handleEvents(bool &running, player &p, int &new_x, int &new_y) {
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            running = false;
        }

        if(event.type == SDL_KEYDOWN) {
            switch(event.key.keysym.sym) {
                case SDLK_UP:
                    new_y -= p.speed;
                    p.direction = 3;
                    break;
                case SDLK_DOWN:
                    new_y += p.speed;
                    p.direction = 0;
                    break;
                case SDLK_LEFT:
                    new_x -= p.speed;
                    p.direction = 2;
                    break;
                case SDLK_RIGHT:
                    new_x += p.speed;
                    p.direction = 1;
                    break;
            }
            p.currentFrame = (SDL_GetTicks() / 100) % p.frameCount;
        }
    }
}

void update(player &p, int &new_x, int &new_y, tileMap &map, warning &warning_) {
    int tile_x = new_x / map.tileSize;
    int tile_y = new_y / map.tileSize;
    int tile_x2 = (new_x + p.width - 1) / map.tileSize;
    int tile_y2 = (new_y + p.height - 1) / map.tileSize;
    
    if (tile_x >= 0 && tile_x2 < map.mapWidth && tile_y >= 0 && tile_y2 < map.mapHeight) {
        if (map.gameMap[tile_y][tile_x] != 0 && map.gameMap[tile_y][tile_x2] != 0 &&
            map.gameMap[tile_y2][tile_x] != 0 && map.gameMap[tile_y2][tile_x2] != 0) {
            
            warning_.direction += (abs(new_x - p.x) + abs(new_y - p.y))/10;
            p.x = new_x;    
            p.y = new_y;
        }
    }
    else{
        
    }
}

void renderPlayer(SDL_Renderer* &renderer, SDL_Rect &camera, player &p){

    SDL_Rect playerRect = { p.x - camera.x, p.y - camera.y, p.width, p.height };

    if (p.texture != NULL) {
        SDL_Rect frameRect;
        frameRect.w = p.frameWidth;
        frameRect.h = p.frameHeight;
        frameRect.x = p.currentFrame * p.frameWidth;
        frameRect.y = p.direction * p.frameHeight;

        SDL_RenderCopy(renderer, p.texture, &frameRect, &playerRect);
    }
}