#ifndef GAME_h_
#define GAME_h_

#include <SDL2/SDL.h>
#include "defines.h"
#include "map.h"
#include "unit.h"
#include "events.h"
#include "utils.h"
#include "player.h"
#include "bullet.h"

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
Bullet *bullets[BUFFER_SIZE * 4] = {NULL};
Uint64 lastTime = 0;
Uint8 running = 1; 

Unit *units[BUFFER_SIZE] = {NULL};
Player player = {.id = 0, .team_id = 0};
Player en_player = {.id = 1, .team_id = 1};
MapCell c_map[MAP_SIZE_Y * MAP_SIZE_X];

void game()
{
    initMouse();
    initMap(c_map);

    for(int i = 0; i < 16; i++)
    {
        for(int j = 0; j < 64; j++)
        {

            spawnUnit(units, BUFFER_SIZE, INFANTRY_MAN, "red", player, 100 + j * 20,  100 + i * 20);
        }
    }

    for(int i = 0; i < 16; i++)
    {
        for(int j = 0; j < 64; j++)
        {

            spawnUnit(units, BUFFER_SIZE, INFANTRY_MAN, "blue", en_player, 100 + j * 20,  800 + i * 20);
        }
    }
    spawnUnit(units, BUFFER_SIZE, INFANTRY_MAN, "red", player, 1280,  720);

    while(running)
    {
        Uint64 currentTime = SDL_GetTicks64();
        Uint64 deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        updateMap(units, BUFFER_SIZE, c_map, MAP_SIZE_X * MAP_SIZE_Y, bullets, BULLET_BUFFER_SIZE);
        processEvents(deltaTime);
        clearBackground();
        
        updateBullets(bullets, BULLET_BUFFER_SIZE, deltaTime);
        updateUnits(units, BUFFER_SIZE, deltaTime);
        destroyOldBullets(bullets, BULLET_BUFFER_SIZE);
        destroyOldUnits(units, BUFFER_SIZE);
        drawMap(renderer, camera, c_map);
        drawUnits(units, BUFFER_SIZE);
        drawBullets(bullets, BULLET_BUFFER_SIZE);
        drawSelectSquare();

        
        
        renderPresent();
        SDL_Delay(1);
    }
    deinitUnits(units, BUFFER_SIZE);
    deInitBullets(bullets, BULLET_BUFFER_SIZE);
}

#endif