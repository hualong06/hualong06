#include "commonfunc.h"
#include "player.h"
#include "base_object.h"
#include "map.h"
#include "gameflow.h"

void initMenu(menu &Menu, SDL_Renderer* &renderer) {
    Menu.textrueMenu = IMG_LoadTexture(renderer, "images/menu.png");
    if (Menu.textrueMenu == NULL) {
        cerr << "Failed to load menu texture: " << IMG_GetError() << endl;
        return;
    }
    Menu.textrueMap = IMG_LoadTexture(renderer, "images/Map.png");
    if (Menu.textrueMap == NULL) {
        cerr << "Failed to load map texture: " << IMG_GetError() << endl;
    }
    if (TTF_Init() == -1) {
        cerr << "Failed to initialize TTF: " << TTF_GetError() << endl;
        return;
    }
    Menu.font = TTF_OpenFont("Pretty Single.otf", 24);
    if (!Menu.font) {
        cerr << "Failed to load font: " << TTF_GetError() << endl;
        return;
    }
}

void renderMenu(SDL_Renderer* &renderer, menu &Menu) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Color white = {255, 255, 255, 255};

    SDL_Surface* startSurface = TTF_RenderText_Solid(Menu.font, "Start", white);
    SDL_Texture* startTexture = SDL_CreateTextureFromSurface(renderer, startSurface);
    SDL_FreeSurface(startSurface);

    SDL_Surface* exitSurface = TTF_RenderText_Solid(Menu.font, "Exit", white);
    SDL_Texture* exitTexture = SDL_CreateTextureFromSurface(renderer, exitSurface);
    SDL_FreeSurface(exitSurface);

    SDL_Rect startRect = {50, SCREEN_HEIGHT - 150, 200, 50};
    SDL_Rect exitRect = {50, SCREEN_HEIGHT - 80, 200, 50};
    SDL_Rect menuRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

    SDL_RenderCopy(renderer, Menu.textrueMenu, NULL, &menuRect);
    SDL_RenderCopy(renderer, startTexture, NULL, &startRect);
    SDL_RenderCopy(renderer, exitTexture, NULL, &exitRect);

    SDL_DestroyTexture(startTexture);
    SDL_DestroyTexture(exitTexture);
}

SDL_Texture* createDarkTexture(SDL_Renderer* renderer) {
    SDL_Surface* darkSurface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);
    if (!darkSurface) {
        cerr << "Failed to create dark surface: " << SDL_GetError() << endl;
        return nullptr;
    } else {
        SDL_FillRect(darkSurface, NULL, SDL_MapRGBA(darkSurface->format, 0, 0, 0, 0));
    }

    SDL_Texture* darkTexture = SDL_CreateTextureFromSurface(renderer, darkSurface);
    SDL_FreeSurface(darkSurface);
    if (!darkTexture) {
        cerr << "Failed to create dark texture: " << SDL_GetError() << endl;
    }
    return darkTexture;
}

void fadeIn(SDL_Renderer* renderer, SDL_Texture* darkTexture, bool& running, icon& gold, menu& Menu, string status) {
    Uint8 alpha = 0;
    bool fading = true;
    bool endMenuRunning = true;

    while (endMenuRunning && fading) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                endMenuRunning = false;
                fading = false;
            }
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_1:
                        endMenuRunning = false;
                        fading = false;
                        break;
                    case SDLK_2:
                        running = false;
                        endMenuRunning = false;
                        fading = false;
                        break;
                }
            }
        }

        SDL_SetTextureAlphaMod(darkTexture, alpha);
        SDL_RenderCopy(renderer, darkTexture, NULL, NULL);

        renderEndGameMenu(renderer, status, gold, Menu);

        SDL_RenderPresent(renderer);

        if (alpha < 200) {
            alpha += 5;
        } else {
            fading = false;
        }
    }
}

void renderEndGameMenu(SDL_Renderer* renderer, string status, icon gold, menu Menu) {
    SDL_Color white = {255, 255, 255};

    TTF_Font* largeFont = TTF_OpenFont("Pretty Single.otf", 48);
    if (!largeFont) {
        cerr << "Failed to load large font: " << TTF_GetError() << endl;
        return;
    }

    SDL_Surface* surface = TTF_RenderText_Solid(largeFont, status.c_str(), white);
    if (!surface) {
        cerr << "Failed to create surface for status text: " << TTF_GetError() << endl;
        TTF_CloseFont(largeFont);
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        cerr << "Failed to create texture for status text: " << SDL_GetError() << endl;
        TTF_CloseFont(largeFont);
        return;
    }

    int textWidth = 0, textHeight = 0;
    TTF_SizeText(largeFont, status.c_str(), &textWidth, &textHeight);
    SDL_Rect textRect = {(SCREEN_WIDTH - textWidth) / 2, (SCREEN_HEIGHT - textHeight) / 2 - 60, textWidth, textHeight};
    SDL_RenderCopy(renderer, texture, NULL, &textRect);
    SDL_DestroyTexture(texture);

    TTF_CloseFont(largeFont);

    surface = TTF_RenderText_Solid(Menu.font, ("Your Gold: " + to_string(gold.count)).c_str(), white);
    if (!surface) {
        cerr << "Failed to create surface for gold text: " << TTF_GetError() << endl;
        return;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        cerr << "Failed to create texture for gold text: " << SDL_GetError() << endl;
        return;
    }

    TTF_SizeText(Menu.font, ("Your Gold: " + to_string(gold.count)).c_str(), &textWidth, &textHeight);
    SDL_Rect goldTextRect = {(SCREEN_WIDTH - textWidth) / 2, (SCREEN_HEIGHT - textHeight) / 2, textWidth, textHeight};
    SDL_RenderCopy(renderer, texture, NULL, &goldTextRect);
    SDL_DestroyTexture(texture);

    surface = TTF_RenderText_Solid(Menu.font, "1. Play Again", white);
    if (!surface) {
        cerr << "Failed to create surface for Play Again text: " << TTF_GetError() << endl;
        return;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        cerr << "Failed to create texture for Play Again text: " << SDL_GetError() << endl;
        return;
    }

    TTF_SizeText(Menu.font, "1. Play Again", &textWidth, &textHeight);
    SDL_Rect playAgainRect = {(SCREEN_WIDTH - textWidth) / 2, (SCREEN_HEIGHT - textHeight) / 2 + 30, textWidth, textHeight};
    SDL_RenderCopy(renderer, texture, NULL, &playAgainRect);
    SDL_DestroyTexture(texture);

    surface = TTF_RenderText_Solid(Menu.font, "2. Exit Game", white);
    if (!surface) {
        cerr << "Failed to create surface for Exit Game text: " << TTF_GetError() << endl;
        return;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        cerr << "Failed to create texture for Exit Game text: " << SDL_GetError() << endl;
        return;
    }

    TTF_SizeText(Menu.font, "2. Exit Game", &textWidth, &textHeight);
    SDL_Rect exitGameRect = {(SCREEN_WIDTH - textWidth) / 2, (SCREEN_HEIGHT - textHeight) / 2 + 60, textWidth, textHeight};
    SDL_RenderCopy(renderer, texture, NULL, &exitGameRect);
    SDL_DestroyTexture(texture);
}

void handleMenuEvents(SDL_Renderer* renderer, bool& running, GameState& state) {
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
}

void handleEndGameEvents(SDL_Renderer* renderer, bool& running, GameState& state, player& p, tileMap& map, icon& gold, menu& Menu, object &warning) {
    bool endMenuRunning = true;
    while (endMenuRunning) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                endMenuRunning = false;
            }
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_1:
                        state = PLAYING;
                        endMenuRunning = false;
                        resetGame(renderer, p, warning, gold, map, state);
                        break;
                    case SDLK_2:
                        running = false;
                        endMenuRunning = false;
                        break;
                }
            }
        }

        SDL_RenderPresent(renderer);
    }
}