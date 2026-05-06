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
static int frames = 0;
Uint64 fpsTimer = 0;

size_t totalUnitsMem = (size_t)MAP_SIZE_X * MAP_SIZE_Y * MAX_UNITS_IN_CELL * sizeof(int);
size_t totalBulletsMem = (size_t)MAP_SIZE_X * MAP_SIZE_Y * MAX_BULLETS_IN_CELL * sizeof(int);

void game()
{
    printf("totalUnitsMem: %zu\n", totalUnitsMem);
    printf("totalBulletsMem: %zu\n", totalBulletsMem);
    initMouse();
    initMap(c_map);

    for(int i = 0; i < 16; i++)
    {
        for(int j = 0; j <  16; j++)
        {

            spawnUnit(units, BUFFER_SIZE, INFANTRY_MAN, "red", player, 100 + j * 20,  100 + i * 20);
        }
    }

    // for(int i = 0; i < 16; i++)
    // {
    //     for(int j = 0; j < 64; j++)
    //     {

    //         spawnUnit(units, BUFFER_SIZE, INFANTRY_MAN, "blue", en_player, 100 + j * 20,  800 + i * 20);
    //     }
    // }
    int *arr = (int *)malloc(sizeof(int) * 1);
    if(!arr)
    {
        printf("Failed!\n");
        exit(0);
    }

    while(running)
    {
        frames++;
        Uint64 currentTime = SDL_GetTicks64();
        Uint64 deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        fpsTimer += deltaTime;
        updateMap(units, BUFFER_SIZE, c_map, MAP_SIZE_X * MAP_SIZE_Y, bullets, BULLET_BUFFER_SIZE);
        processEvents(deltaTime);
        clearBackground();
        
        updateBulletsMT(bullets, BULLET_BUFFER_SIZE, deltaTime);
        updateUnitsMT(units, BUFFER_SIZE, deltaTime);
        destroyOldBullets(bullets, BULLET_BUFFER_SIZE);
        destroyOldUnits(units, BUFFER_SIZE);
        drawMap(renderer, camera, c_map);
        drawUnits(units, BUFFER_SIZE);
        drawBullets(bullets, BULLET_BUFFER_SIZE);
        drawSelectSquare();

        
        if(fpsTimer > 1000)
        {
            printf("fps: %d\n", frames);
            fpsTimer = 0;
            frames = 0;
        }

        renderPresent();
        SDL_Delay(1);
    }
    deinitUnits(units, BUFFER_SIZE);
    deInitBullets(bullets, BULLET_BUFFER_SIZE);
    free(arr);
}

#endif