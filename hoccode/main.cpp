#define SDL_MAIN_HANDLED
#include <iostream>
#include <string>
#include <time.h>
#include <vector>
#include <fstream>
#include <SDL2\SDL.h>
#include <SDL2\SDL_image.h>

using namespace std;

const int SCREEN_WIDTH = 16*80;
const int SCREEN_HEIGHT = 16*50;

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
    string filename;
    SDL_Texture* texture;
};

struct warning {
    int x, y;
    int width, height;
    int frameWidth;
    int frameHeight;
    int direction;
    SDL_Texture* texture;
};

struct icon {
    int x, y;
    int width, height;
    int frameWidth;
    int frameHeight;
    int currentFrame;
    int frameCount;
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

void initTileMap(tileMap &map, SDL_Renderer* &renderer) {
    map.tileSize = 16;
    map.mapWidth = 16*120*3/2;
    map.mapHeight = 16*67*3/2;
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

void initwarning(warning &warning_) {
    warning_.x = 334;
    warning_.y = 0;
    warning_.width = 100;
    warning_.height = 10;
    warning_.direction = 0;
}

void initIcon(icon &icon_, SDL_Renderer* &renderer) {
    icon_.x = 7*64;
    icon_.y = 4*64;
    icon_.width = 64;
    icon_.height = 64;
    icon_.frameWidth = 320;
    icon_.frameHeight = 320;
    icon_.currentFrame = 0;
    icon_.frameCount = 4;
    icon_.texture = IMG_LoadTexture(renderer, "suggest.png");
    if(icon_.texture == NULL) {
        cerr << "Failed to load icon texture SDL_Error:" << SDL_GetError() << endl;
    }
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
}

/*
void endGame(SDL_Renderer* renderer, warning &warning_, string &status, tileMap &map, player &p) {
    if(warning_.direction > 100) {
        status = "Game_Over";
        renderEndGame(renderer, status);
    }
    else if(warning_.direction <= 100 && map.gameMap[p.y/64][p.x/64] == 9) {
        status = "You_Win";
        rederEngGame(renderer, status);
    }

}
*/
void renderInteract(SDL_Renderer* &renderer, icon &icon_) {
    SDL_Rect iconRect = {icon_.x, icon_.y, icon_.width, icon_.height};
    if(icon_.texture != NULL) {
        SDL_Rect frameRect;
        frameRect.w = icon_.frameWidth;
        frameRect.h = icon_.frameHeight;
        frameRect.x = icon_.currentFrame * icon_.frameWidth;
        SDL_RenderCopy(renderer, icon_.texture, &frameRect, &iconRect);
    }
//    icon_.currentFrame = SDL_GetTicks() % icon_.frameCount;

    SDL_RenderPresent(renderer);
}

void renderEndGame(SDL_Renderer* &renderer, string &status) {
    

    SDL_RenderPresent(renderer);

}

void render(SDL_Renderer* &renderer, player &p, object &safe, warning &warning_, tileMap &map) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_Rect mapRect = {-16*90, -16*50, map.mapWidth, map.mapHeight};
    if(map.texture != NULL) {
        SDL_RenderCopy(renderer, map.texture, NULL, &mapRect);
    }
    /*
    SDL_Rect safeRect = {safe.x, safe.y, safe.width, safe.height};
    if(safe.texture != NULL) {

        SDL_RenderCopy(renderer, safe.texture, NULL, &safeRect); 
    }
    */
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

void interact(SDL_Renderer *renderer, player &p, tileMap &map, string &status, icon &icon_) {
    if(map.gameMap[p.y/64][p.x/64] == 2) {
        status = "True";
        renderInteract(renderer, icon_);
    }

}

void clean(SDL_Window* &window, SDL_Renderer* &renderer, player &p, object &safe, tileMap &map, icon &icon_) {
    if(p.texture != NULL) {
        SDL_DestroyTexture(p.texture);
        p.texture = NULL;
    }
    if(safe.texture != NULL) {
        SDL_DestroyTexture(safe.texture);
        safe.texture = NULL;
    }
    if(icon_.texture != NULL) {
        SDL_DestroyTexture(icon_.texture);
        icon_.texture = NULL;
    }
    for (int i = 0; i < map.mapHeight/map.tileSize; i++) {
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
    icon icon_;
    
    initPlayer(p, renderer);
    initObject(safe, renderer);
    initwarning(warning_);
    initTileMap(map, renderer);
    initIcon(icon_, renderer);

    if (!loadMap(map)) {  
        cerr << "Failed to load map!" << endl;
        return;
    }

    bool running = true;
    string satatus = "False";

    while (running) {
        int new_x = p.x;
        int new_y = p.y;
        handleEvents(running, p, new_x, new_y);
        update(p, new_x, new_y, map, warning_);
        render(renderer, p, safe, warning_, map);
        interact(renderer, p, map, satatus, icon_);
        SDL_Delay(1000 / 32);
    }

    clean(window, renderer, p, safe, map, icon_);

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

