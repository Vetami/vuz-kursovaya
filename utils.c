#include "utils.h"
#include "events.h"

SDL_Color bgColor = {0, 0, 0, 255};

void clearBackground()
{
    SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderClear(renderer);
}

void renderPresent()
{
    SDL_RenderPresent(renderer);
    SDL_Delay(16);
}

void initializeGame()
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Failed to initialize SDL!\n");
        exit(0);
    }
    window   = SDL_CreateWindow("RTS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}