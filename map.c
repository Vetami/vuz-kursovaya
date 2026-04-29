#include "defines.h"
#include <SDL2/SDL.h>
#include "map.h"

char map[MAP_SIZE_Y][MAP_SIZE_X] = 
                 {"11111111",
                  "10000001",
                  "10000001",
                  "10000001",
                  "10000001",
                  "10000001",
                  "10000001",
                  "11111111",
                };

void drawMap(SDL_Renderer *renderer, const SDL_Rect camera)
{
    for(int i = 0; i < MAP_SIZE_Y; i++)
    {
        for(int j = 0; j < MAP_SIZE_X; j++)
        {
            char cell = map[i][j];
            SDL_Color color;
            if(cell == '1')
            {
                color.r = 255;
                color.g = 0;
                color.b = 0;
                color.a = 255;
            }
            else if(cell == '0')
            {
                color.r = 255;
                color.g = 255;
                color.b = 255;
                color.a = 255;
            }
            else{
                color.r = 0;
                color.g = 255;
                color.b = 0;
                color.a = 255;
            }
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_Rect renderQuad = {
                j * CELL_SIZE - camera.x,
                i * CELL_SIZE - camera.y,
                CELL_SIZE,
                CELL_SIZE
            };
            SDL_RenderFillRect(renderer, &renderQuad);
        }
    }
}