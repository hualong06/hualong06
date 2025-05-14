#include "commonfunc.h"
#include "player.h"
#include "base_object.h"
#include "map.h"
#include "gameflow.h" 

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

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << endl;
        return false;
    }

    return true;
}

void clean(SDL_Window* &window, SDL_Renderer* &renderer, player &p, tileMap &map, icon &icon_, menu &Menu) {
    if (p.texture != NULL) {
        SDL_DestroyTexture(p.texture);
        p.texture = NULL;
    }
    if (p.diagonalTexture != NULL) {
        SDL_DestroyTexture(p.diagonalTexture);
        p.diagonalTexture = NULL;
    }
    if (icon_.texture != NULL) {
        SDL_DestroyTexture(icon_.texture);
        icon_.texture = NULL;
    }
    if (map.textureLayer1 != NULL) {
        SDL_DestroyTexture(map.textureLayer1);
        map.textureLayer1 = NULL;
    }
    if (map.textureLayer2 != NULL) {
        SDL_DestroyTexture(map.textureLayer2);
        map.textureLayer2 = NULL;
    }
    if (map.textureLayer3 != NULL) {
        SDL_DestroyTexture(map.textureLayer3);
        map.textureLayer3 = NULL;
    }
    if (map.textureLayer4 != NULL) {
        SDL_DestroyTexture(map.textureLayer4);
        map.textureLayer4 = NULL;
    }
    for (int i = 0; i < map.mapHeight / map.tileSize; i++) {
        delete[] map.gameMap[i];
    }
    delete[] map.gameMap;
    for (int  i = 0; i < 9; i++) {
        delete[] map.mapGold[i];
    }
    delete[] map.mapGold;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void gameLoop(SDL_Window* &window, SDL_Renderer* &renderer) {
    player p;
    object safe;
    object warning;
    tileMap map;
    icon icon_;
    icon gold;
    menu Menu;

    initPlayer(p, renderer);
    initObject(safe, renderer);
    initwarning(warning);
    initTileMap(map, renderer);
    initIcon(icon_, renderer);
    initMenu(Menu, renderer);
    initGold(gold, renderer);

    loadMap(map);

    bool running = true;
    GameState state = MENU;
    string status = "False";

    int new_x = p.x;
    int new_y = p.y;
    SDL_Texture* darkTexture = createDarkTexture(renderer);

    while (running) {

        if (state == MENU) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderClear(renderer);
            renderMenu(renderer, Menu);
            SDL_RenderPresent(renderer);
            handleMenuEvents(renderer, running, state);
        } else if (state == PLAYING) {
            SDL_Event event;

            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    running = false;
                }

                if (event.type == SDL_KEYDOWN) {
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            running = false;
                            break;
                        default:
                            break;
                    }
                }
                checkAndChangeRoom(renderer, p, map, event);
                updateGold(renderer, p, map, gold, event);

            }

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderClear(renderer);
            render(renderer, p, map);
            renderGold(renderer, gold, Menu);
            renderWarning(renderer, warning);
            renderGoldProgressBar(renderer, gold, 350);
            SDL_RenderPresent(renderer);

            update(p, new_x, new_y, map);
            handleMovement(p, new_x, new_y);
            checkEndGame(p, map, warning, state, status);
            SDL_Delay(1000/32);

        } else if (state == ENDGAME) {

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderClear(renderer);
            if (darkTexture) {
                fadeIn(renderer, darkTexture, running, gold, Menu, status);
                SDL_DestroyTexture(darkTexture);
            }
            
            handleEndGameEvents(renderer, running, state, p, map, gold, Menu, warning);
        }
        
    }
    TTF_CloseFont(Menu.font);
    TTF_Quit();
    clean(window, renderer, p, map, icon_, Menu);
    if (darkTexture) {
        SDL_DestroyTexture(darkTexture);
    }
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