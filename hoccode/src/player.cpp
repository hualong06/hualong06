#include "commonfunc.h"
#include "player.h"
#include "base_object.h"
#include "map.h"

void initPlayer(player &p, SDL_Renderer* &renderer) {
    p.x = 30 * 16;
    p.y = 20 * 16;
    p.width = 93 * 4 / 3;
    p.height = 96 * 4 / 3;
    p.speed = 16;

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

void handleEvents(bool &running, player &p, int &new_x, int &new_y) {
    new_x = p.x;
    new_y = p.y;

    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
    }

    const Uint8* keystate = SDL_GetKeyboardState(NULL);

    if (keystate[SDL_SCANCODE_UP] && keystate[SDL_SCANCODE_LEFT]) {
        new_y -= p.speed;
        new_x -= p.speed;
        p.direction = 5; // Hướng chéo trên-trái
    } else if (keystate[SDL_SCANCODE_UP] && keystate[SDL_SCANCODE_RIGHT]) {
        new_y -= p.speed;
        new_x += p.speed;
        p.direction = 7; // Hướng chéo trên-phải
    } else if (keystate[SDL_SCANCODE_DOWN] && keystate[SDL_SCANCODE_LEFT]) {
        new_y += p.speed;
        new_x -= p.speed;
        p.direction = 6; // Hướng chéo dưới-trái
    } else if (keystate[SDL_SCANCODE_DOWN] && keystate[SDL_SCANCODE_RIGHT]) {
        new_y += p.speed;
        new_x += p.speed;
        p.direction = 4; // Hướng chéo dưới-phải
    } else if (keystate[SDL_SCANCODE_UP]) {
        new_y -= p.speed;
        p.direction = 3; // Hướng lên
    } else if (keystate[SDL_SCANCODE_DOWN]) {
        new_y += p.speed;
        p.direction = 0; // Hướng xuống
    } else if (keystate[SDL_SCANCODE_LEFT]) {
        new_x -= p.speed;
        p.direction = 2; // Hướng trái
    } else if (keystate[SDL_SCANCODE_RIGHT]) {
        new_x += p.speed;
        p.direction = 1; // Hướng phải
    }

    if (keystate[SDL_SCANCODE_UP] || keystate[SDL_SCANCODE_DOWN] || 
        keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_RIGHT]) {
        p.currentFrame = (SDL_GetTicks() / 100) % p.frameCount;
    }
}
void update(player &p, int &new_x, int &new_y, tileMap &map, warning &warning_) {

    int tile_x1 = new_x / map.tileSize;
    int tile_y1 = new_y / map.tileSize;
    int tile_x2 = (new_x + p.width) / map.tileSize;
    int tile_y2 = (new_y + p.height) / map.tileSize;

    if (map.gameMap[tile_y2][tile_x1] != 0 && map.gameMap[tile_y2][tile_x2] != 0) {

        warning_.direction += (abs(new_x - p.x) + abs(new_y - p.y)) / 100;
        p.x = new_x;
        p.y = new_y;
    }

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