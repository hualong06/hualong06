#define SDL_MAIN_HANDLED
#include <iostream>
#include <SDL2\SDL.h>
#include <SDL2\SDL_image.h>

using namespace std;

const int SCREEN_WIDTH = 768;
const int SCREEN_HEIGHT = 640;

struct player {
    int x, y;
    int width, height;
    int speed;
    int currentFrame;
    int frameCount;
    int direction;
    int frameWidth;
    int frameHeight;
    SDL_Texture* texture;
};

struct object {
    int x, y;
    int width, height;
    int frameWidth;
    int frameHeight;
    SDL_Texture* texture;
};

bool init(SDL_Window* &window, SDL_Renderer* &renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }

    window = SDL_CreateWindow("Game_1.0", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    
    if (window == NULL) {
        cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }

    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << endl;
        return false;
    }

    return true;
}

void initPlayer(player &p, SDL_Renderer* &renderer) {
    p.x = 0;
    p.y = 0;
    p.width = 93;
    p.height = 96;
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

void initObject(object &safe, SDL_Renderer* &renderer) {
    safe.x = 500;
    safe.y = 0;
    safe.width = 240;
    safe.height = 300;
    safe.texture = IMG_LoadTexture(renderer, "safe.jpg");
    if(safe.texture == NULL) {
        cerr << "Failed to load fase textrue SDL_Error:" << SDL_GetError() << endl;
    }
    safe.frameWidth = 811;
    safe.frameHeight = 1011;
}

void handleEvents(bool &running, player &p) {
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_UP:
                    p.y -= p.speed;
                    p.direction = 3;
                    break;
                case SDLK_DOWN:
                    p.y += p.speed;
                    p.direction = 0;
                    break;
                case SDLK_LEFT:
                    p.x -= p.speed;
                    p.direction = 2;
                    break;
                case SDLK_RIGHT:
                    p.x += p.speed;
                    p.direction = 1;
                    break;
            }
            p.currentFrame = (SDL_GetTicks() / 100) % p.frameCount;
        }
    }
   
}

void update(player &p ) {
    if (p.x < 0) {
        p.x = 0;
    }
    if (p.y < 0) {
        p.y = 0;
    }
    if (p.x + p.width > SCREEN_WIDTH) {
        p.x = SCREEN_WIDTH - p.width;
    }
    if (p.y + p.height > SCREEN_HEIGHT) {
        p.y = SCREEN_HEIGHT - p.height;
    }   
}

void render(SDL_Renderer* &renderer, player &p, object &safe) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_Rect safeRect = {safe.x, safe.y, safe.width, safe.height};
    if(safe.texture != NULL) {

        SDL_RenderCopy(renderer, safe.texture, NULL, &safeRect); 
    }

    SDL_Rect playerRect = { p.x, p.y, p.width, p.height };
    if (p.texture != NULL) {
        SDL_Rect frameRect;
        frameRect.w = p.frameWidth;
        frameRect.h = p.frameHeight;
        frameRect.x = p.currentFrame * p.frameWidth;
        frameRect.y = p.direction * p.frameHeight;

        SDL_RenderCopy(renderer, p.texture, &frameRect, &playerRect);

    }
    SDL_RenderPresent(renderer);
}

void clean(SDL_Window* &window, SDL_Renderer* &renderer, player &p) {
    if(p.texture != NULL) {
        SDL_DestroyTexture(p.texture);
        p.texture = NULL;
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void gameLoop(SDL_Window* &window, SDL_Renderer* &renderer) {
    player p;
    object safe;
    initPlayer(p, renderer);
    initObject(safe, renderer);
    bool running = true;

    while (running) {
        handleEvents(running, p);
        update(p);
        render(renderer, p, safe);
        SDL_Delay(16);
    }

    clean(window, renderer, p);

}

int main(int argc, char* argv[]) {
    
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    if (!init(window, renderer)) {
        return -1;
    }

    gameLoop(window, renderer);

    return 0;
    
}

