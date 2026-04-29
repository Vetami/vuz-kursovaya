#ifndef UTILS_h_
#define UTILS_h_
#include <SDL2/SDL.h>
#include "defines.h"

extern SDL_Renderer *renderer;
extern SDL_Window *window;

void clearBackground();
void renderPresent();
void initializeGame();

#endif