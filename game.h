#ifndef GAME_h_
#define GAME_h_

#include <SDL2/SDL.h>
#include "defines.h"
#include "map.h"
#include "unit.h"
#include "events.h"
#include "utils.h"

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
Uint8 running = 1;

Unit *units[BUFFER_SIZE];

void game()
{
    spawnUnit(units, BUFFER_SIZE, Archer, "blue", 0, 0, 0);
    spawnUnit(units, BUFFER_SIZE, Archer, "blue", 0, 100, 0);
    spawnUnit(units, BUFFER_SIZE, Archer, "blue", 0, 40, 0);
    while(running)
    {
        processEvents();
        clearBackground();
        
        updateUnits(units, BUFFER_SIZE);
        // drawMap(renderer, camera);
        drawUnits(units, BUFFER_SIZE);
        drawSelectSquare();
        

        renderPresent();
    }
}

#endif