#ifndef STRUCTS_H_
#define STRUCTS_H_

#include <SDL2/SDL.h>
#include "defines.h"

typedef struct{
    float x, y;
} vec2;

typedef struct 
{
    SDL_Rect renderRect;
    SDL_Color color;
    SDL_Texture *texture;
    char type_terrain;
    int units[MAX_UNITS_IN_CELL];
    int max_units;
    int cur_units;
    int bullets[MAX_BULLETS_IN_CELL];
    int max_bullets;
    int cur_bullets;
    int width;
    int height;
    int x, y;
}
MapCell;

#endif