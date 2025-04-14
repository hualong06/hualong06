#include "commonfunc.h"
#include "player.h"
#include "base_object.h"
#include "map.h"


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
    GameState state = MENU;
    string status = "False";

    int new_x = p.x;
    int new_y = p.y;

    if (TTF_Init() == -1) {
        cerr << "Failed to initialize TTF: " << TTF_GetError() << endl;
        return;
    }
    TTF_Font* font = TTF_OpenFont("arial.ttf", 24);
    if (!font) {
        cerr << "Failed to load font: " << TTF_GetError() << endl;
        return;
    }

    while (running) {
        if (state == MENU) {
            renderMenu(renderer, font);

            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    running = false;
                }
                if (event.type == SDL_KEYDOWN) {
                    switch (event.key.keysym.sym) {
                        case SDLK_1:
                            state = PLAYING;
                            break;
                        case SDLK_2:
                            running = false;
                            break;
                    }
                }
            }
        } else if (state == PLAYING) {
            handleEvents(running, p, new_x, new_y);
            update(p, new_x, new_y, map, warning_);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderClear(renderer);
            renderMap(renderer, map, p);
            renderWarning(renderer, warning_);
            renderPlayer(renderer, p);
            SDL_RenderPresent(renderer);
            SDL_Delay(1000 / 32);
        }
    }

    TTF_CloseFont(font);
    TTF_Quit();
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

