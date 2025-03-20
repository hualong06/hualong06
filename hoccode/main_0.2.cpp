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

struct tileMap {
    int tileSize;
    int mapWidth;
    int mapHeight;
    int **gameMap;
};

struct warning {
    int x, y;
    int width, height;
    int frameWidth;
    int frameHeight;
    int direction;
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
    p.x = 100;
    p.y = 100;
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
    safe.x = 6*64;
    safe.y = 4*64;
    safe.width = 128;
    safe.height = 128;
    safe.texture = IMG_LoadTexture(renderer, "safe.jpg");
    if(safe.texture == NULL) {
        cerr << "Failed to load fase textrue SDL_Error:" << SDL_GetError() << endl;
    }
    safe.frameWidth = 811;
    safe.frameHeight = 1011;
}

void initTileMap(tileMap &map) {
    map.tileSize = 64;
    map.mapWidth = 12;
    map.mapHeight = 10;
    map.gameMap = new int*[map.mapHeight];
    for (int i = 0; i < map.mapHeight; i++) {
        map.gameMap[i] = new int[map.mapWidth];
    }
    int map_[map.mapHeight][map.mapWidth] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {9, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1},
        {9, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };
    for (int i = 0; i < map.mapHeight; i++) {
        for (int j = 0; j < map.mapWidth; j++) {
            map.gameMap[i][j] = map_[i][j];
        }
    }
}

void initwarning(warning &warning_) {
    warning_.x = 334;
    warning_.y = 0;
    warning_.width = 100;
    warning_.height = 10;
    warning_.direction = 0;
}

void handleEvents(bool &running, player &p, int &new_x, int &new_y) {
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
/*
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
        */
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
        if (map.gameMap[tile_y][tile_x] != 1 && map.gameMap[tile_y][tile_x2] != 1 &&
            map.gameMap[tile_y2][tile_x] != 1 && map.gameMap[tile_y2][tile_x2] != 1) {
            
            warning_.direction += (abs(new_x - p.x) + abs(new_y - p.y))/10;
            p.x = new_x;    
            p.y = new_y;
        }
    }
}


/*
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
*/
void render(SDL_Renderer* &renderer, player &p, object &safe, warning &warning_) {
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

    SDL_Rect warningRect = {warning_.x, warning_.y, warning_.direction, warning_.height};
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &warningRect);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &warningRect);

    SDL_RenderPresent(renderer);
}

void clean(SDL_Window* &window, SDL_Renderer* &renderer, player &p, object &safe, tileMap &map) {
    if(p.texture != NULL) {
        SDL_DestroyTexture(p.texture);
        p.texture = NULL;
    }
    if(safe.texture != NULL) {
        SDL_DestroyTexture(safe.texture);
        safe.texture = NULL;
    }
    for (int i = 0; i < map.mapHeight; i++) {
        delete[] map.gameMap[i];
    }
    delete[] map.gameMap;    

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void gameLoop(SDL_Window* &window, SDL_Renderer* &renderer) {
    player p;
    object safe;
    warning warning_;
    tileMap map;
    
    initPlayer(p, renderer);
    initObject(safe, renderer);
    initwarning(warning_);
    initTileMap(map);
    bool running = true;

    while (running && warning_.direction < 100) {
        int new_x = p.x;
        int new_y = p.y;
        handleEvents(running, p, new_x, new_y);
        update(p, new_x, new_y, map, warning_);
        render(renderer, p, safe, warning_);
        SDL_Delay(1000 / 32);
    }

    clean(window, renderer, p, safe, map);

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
