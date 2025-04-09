#include "commonfunc.h"
#include "base_object.h"
#include "map.h"
#include "player.h"

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