#include "commonfunc.h"
#include "player.h"
#include "base_object.h"
#include "map.h"

void initPlayer(player &p, SDL_Renderer* &renderer) {
    p.x = 20 * 16;
    p.y = 23 * 16;
    p.width = 93;
    p.height = 96 * 7 / 8;
    p.speed = 16 * 7 / 8;

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
        p.direction = 5; 
    } else if (keystate[SDL_SCANCODE_UP] && keystate[SDL_SCANCODE_RIGHT]) {
        new_y -= p.speed;
        new_x += p.speed;
        p.direction = 7; 
    } else if (keystate[SDL_SCANCODE_DOWN] && keystate[SDL_SCANCODE_LEFT]) {
        new_y += p.speed;
        new_x -= p.speed;
        p.direction = 6; 
    } else if (keystate[SDL_SCANCODE_DOWN] && keystate[SDL_SCANCODE_RIGHT]) {
        new_y += p.speed;
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
void update(player &p, int &new_x, int &new_y, tileMap &map, object &warning) {

    int tileX = (p.x + p.width / 2) / map.tileSize;
    int tileY = (p.y + p.height) / map.tileSize;

    if (map.gameMap[tileY][tileX] != 0) {

        warning.direction += (abs(new_x - p.x) + abs(new_y - p.y)) / 100;
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