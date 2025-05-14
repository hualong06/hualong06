#include "commonfunc.h"
#include "base_object.h"
#include "map.h"
#include "player.h"
#include "gameflow.h"

void initObject(object &safe, SDL_Renderer* &renderer) {
    safe.x = 6*64;
    safe.y = 4*64;
    safe.width = 128;
    safe.height = 128;
    safe.texture = IMG_LoadTexture(renderer, "images/safe.jpg");
    if(safe.texture == NULL) {
        cerr << "Failed to load fase textrue SDL_Error:" << SDL_GetError() << endl;
    }
    safe.frameWidth = 811;
    safe.frameHeight = 1011;
}

void initwarning(object &warning) {
    warning.x = 20;
    warning.y = 20;
    warning.width = 100;
    warning.height = 10;
    warning.direction = 0;
}

void initIcon(icon &icon_, SDL_Renderer* &renderer) {
    icon_.x = 7*64;
    icon_.y = 4*64;
    icon_.width = 64;
    icon_.height = 64;
    icon_.frameWidth = 320;
    icon_.frameHeight = 320;
    icon_.currentFrame = 0;
    icon_.count = 4;
    icon_.texture = IMG_LoadTexture(renderer, "images/suggest.png");
    if(icon_.texture == NULL) {
        cerr << "Failed to load icon texture SDL_Error:" << SDL_GetError() << endl;
    }
}

void initGold(icon &gold, SDL_Renderer* &renderer){
    gold.x = 20;
    gold.y = 20;
    gold.width = 32;
    gold.height = 32;
    gold.count = 0;
    gold.texture = IMG_LoadTexture(renderer, "images/gold.png");
    if(gold.texture == NULL) {
        cerr << "Failed to load gold texture SDL_Error:" << SDL_GetError() << endl;
    }
}

void renderWarning(SDL_Renderer* &renderer, object &warning) {
    static Uint32 previousTime = 0;
    Uint32 currentTime = SDL_GetTicks();
    Uint32 deltaTime = currentTime - previousTime;

    warning.direction += deltaTime / 50;
    if (warning.direction > 255) {
        warning.direction = 255;
    }

    previousTime = currentTime;

    SDL_Rect warningRect = {warning.x, warning.y, warning.direction, warning.height};

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &warningRect);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &warningRect);
}

bool checkInteraction(const SDL_Event& event) {
    return event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_f;
}

void renderGold(SDL_Renderer* &renderer, icon &gold, menu &Menu) {
    gold.x = 20;
    gold.y = 50;

    SDL_Rect goldRect = {gold.x, gold.y, gold.width, gold.height};
    SDL_RenderCopy(renderer, gold.texture, NULL, &goldRect);

    string goldText = to_string(gold.count);
    SDL_Color white = {255, 255, 255, 255};

    SDL_Surface* surface = TTF_RenderText_Solid(Menu.font, goldText.c_str(), white);
    if (!surface) {
        cerr << "Failed to create surface for gold count: " << TTF_GetError() << endl;
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        cerr << "Failed to create texture for gold count: " << SDL_GetError() << endl;
        SDL_FreeSurface(surface);
        return;
    }

    int textWidth, textHeight;
    TTF_SizeText(Menu.font, goldText.c_str(), &textWidth, &textHeight);

    SDL_Rect goldCountRect = {
        gold.x + gold.width + 10,
        gold.y + (gold.height - textHeight) / 2,
        textWidth,
        textHeight
    };

    SDL_RenderCopy(renderer, texture, NULL, &goldCountRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void updateGold(SDL_Renderer* &renderer, player &p, tileMap &map, icon &gold, SDL_Event &event) {
    int tileX = (p.x + p.width / 2 - 8) / 64;
    int tileY = (p.y + p.height - 16) / 64;

    if (tileX >= 0 && tileX < 16 && tileY >= 0 && tileY < 9) {
        if (map.mapGold[tileY][tileX] >= 10) {
        }
    } else {
        cout << "Out of bounds access in mapGold: tileX=" << tileX << ", tileY=" << tileY << endl;
    }

    if (map.mapGold[tileY][tileX] > 0) {
        SDL_Texture* texture = IMG_LoadTexture(renderer, "images/TaskArrow.png");
        if (texture != nullptr) {
            SDL_Rect iconRect = { p.x, p.y, 32, 32 };
            SDL_RenderCopy(renderer, texture, NULL, &iconRect);

            if (checkInteraction(event)) { 
                gold.count += map.mapGold[tileY][tileX];
                map.mapGold[tileY][tileX] = 0;
            }
            SDL_DestroyTexture(texture);
        }
    }
}

void renderGoldProgressBar(SDL_Renderer* &renderer, icon &gold, int totalGold) {
    float percentage = (float)gold.count / totalGold;

    int barWidth = 150;
    int barHeight = 20;
    int barX = gold.x;
    int barY = gold.y + gold.height + 10;

    SDL_Rect borderRect = {barX, barY, barWidth, barHeight};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &borderRect);

    SDL_Rect fillRect = {barX, barY, (int)(barWidth * percentage), barHeight};
    SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
    SDL_RenderFillRect(renderer, &fillRect);

    string goldText = to_string(gold.count) + " / " + to_string(totalGold);
    SDL_Color black = {0, 0, 0, 255};

    TTF_Font* font = TTF_OpenFont("Pretty Single.otf", 16);
    if (!font) {
        cerr << "Failed to load font: " << TTF_GetError() << endl;
        return;
    }

    SDL_Surface* surface = TTF_RenderText_Solid(font, goldText.c_str(), black);
    if (!surface) {
        cerr << "Failed to create surface for gold text: " << TTF_GetError() << endl;
        TTF_CloseFont(font);
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        cerr << "Failed to create texture for gold text: " << SDL_GetError() << endl;
        TTF_CloseFont(font);
        return;
    }

    int textWidth, textHeight;
    TTF_SizeText(font, goldText.c_str(), &textWidth, &textHeight);

    SDL_Rect textRect = {
        barX + (barWidth - textWidth) / 2,
        barY + (barHeight - textHeight) / 2,
        textWidth,
        textHeight
    };

    SDL_RenderCopy(renderer, texture, NULL, &textRect);

    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);
}

void checkEndGame(player &p, tileMap &map, object &warning, GameState &state, string &status) {
    int tileX = (p.x + p.width / 2 -8) / map.tileSize;
    int tileY = (p.y + p.height -16) / map.tileSize;

    if(map.gameMap[tileY][tileX] == 11 || warning.direction == 255) {
        if (map.gameMap[tileY][tileX] == 11 && warning.direction < 255) {
            status = "You Win!";
        } else if (warning.direction == 255) {
            status = "Game Over!";
        }
        state = ENDGAME;
    }
}