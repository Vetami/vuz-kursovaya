#include "defines.h"
#include <SDL2/SDL.h>
#include "map.h"

char map[MAP_SIZE_Y][MAP_SIZE_X] = 
                 {0};


void initMap(MapCell *c_map) {
    for(int i = 0; i < MAP_SIZE_Y; i++)
    {
        for (int j = 0; j < MAP_SIZE_X; j++)
        {
            map[i][j] = '0';
        }
        
    }

    for(int i = 0; i < MAP_SIZE_Y; i++)
    {
        for(int j = 0; j < MAP_SIZE_X; j++)
        {
            if(map[i][j] == '1')
            {
                c_map[j + i * MAP_SIZE_X].color.r = 255;
                c_map[j + i * MAP_SIZE_X].color.g = 255;
                c_map[j + i * MAP_SIZE_X].color.b = 255;
                c_map[j + i * MAP_SIZE_X].color.a = 255;
                c_map[j + i * MAP_SIZE_X].type_terrain = 1;
                c_map[j + i * MAP_SIZE_X].cur_units = 0;
                c_map[j + i * MAP_SIZE_X].max_units = MAX_UNITS_IN_CELL;
                c_map[j + i * MAP_SIZE_X].max_bullets = MAX_UNITS_IN_CELL;
                c_map[j + i * MAP_SIZE_X].cur_bullets = 0;
                c_map[j + i * MAP_SIZE_X].width = CELL_SIZE;
                c_map[j + i * MAP_SIZE_X].height = CELL_SIZE;
                c_map[j + i * MAP_SIZE_X].x = j * c_map[j + i * MAP_SIZE_X].width;
                c_map[j + i * MAP_SIZE_X].y = i * c_map[j + i * MAP_SIZE_X].height;
                for(int k = 0; k < MAX_UNITS_IN_CELL; k++)
                {
                    c_map[j + i * MAP_SIZE_X].units[k] = -1;
                }
            }
            if(map[i][j] == '0')
            {
                c_map[j + i * MAP_SIZE_X].color.r = 0;
                c_map[j + i * MAP_SIZE_X].color.g = 255;
                c_map[j + i * MAP_SIZE_X].color.b = 0;
                c_map[j + i * MAP_SIZE_X].color.a = 255;
                c_map[j + i * MAP_SIZE_X].type_terrain = 0;
                c_map[j + i * MAP_SIZE_X].cur_units = 0;
                c_map[j + i * MAP_SIZE_X].max_units = MAX_UNITS_IN_CELL;
                c_map[j + i * MAP_SIZE_X].max_bullets = MAX_UNITS_IN_CELL;
                c_map[j + i * MAP_SIZE_X].cur_bullets = 0;
                c_map[j + i * MAP_SIZE_X].width = CELL_SIZE;
                c_map[j + i * MAP_SIZE_X].height = CELL_SIZE;
                c_map[j + i * MAP_SIZE_X].x = j * c_map[j + i * MAP_SIZE_X].width;
                c_map[j + i * MAP_SIZE_X].y = i * c_map[j + i * MAP_SIZE_X].height;
                for(int k = 0; k < MAX_UNITS_IN_CELL; k++)
                {
                    c_map[j + i * MAP_SIZE_X].units[k] = -1;
                    c_map[j + i * MAP_SIZE_X].bullets[k] = -1;
                }
            }
        }
    }
}

void drawMap(SDL_Renderer *renderer, const SDL_Rect camera, MapCell *c_map)
{
    for(int i = 0; i < MAP_SIZE_Y; i++)
    {
        for(int j = 0; j < MAP_SIZE_X; j++)
        {
            MapCell cell = c_map[j + i * MAP_SIZE_X];
            SDL_SetRenderDrawColor(renderer, cell.color.r, cell.color.g, cell.color.b, cell.color.a);
            SDL_Rect renderQuad = {
                j * cell.width - camera.x,
                i * cell.height - camera.y,
                cell.width,
                cell.height
            };
            SDL_RenderDrawRect(renderer, &renderQuad);
        }
    }
}

void pushId(MapCell *cell, int id)
{
    if(cell->cur_units < cell->max_units)
        cell->units[cell->cur_units++] = id;
}
void pushBId(MapCell *cell, int id)
{
    if(cell->cur_bullets < cell->max_bullets)
        cell->bullets[cell->cur_bullets++] = id;
}

void updateMap(Unit **units, int size, MapCell *map, int m_size, Bullet **bullets, int b_size)
{
    clearMap(map, m_size);
    for(int i = 0; i < size; i++)
    {
        if(units[i])
        {
            Unit *unit = units[i];
            float dx = unit->position.x / CELL_SIZE;
            float dy = unit->position.y / CELL_SIZE;
            int x = (int) dx;
            int y = (int) dy;
            if(dx > 0 && dy > 0 && x >= 0 && y >= 0 && x < MAP_SIZE_X && y < MAP_SIZE_Y)
            {
                pushId(&map[x + y * MAP_SIZE_X], unit->unit_id);
            }
        }
    }
    for(int i = 0; i < b_size; i++)
    {
        if(bullets[i])
        {
            Bullet *bullet = bullets[i];
            float dx = bullet->position.x / CELL_SIZE;
            float dy = bullet->position.y / CELL_SIZE;
            int x = (int) dx;
            int y = (int) dy;
            if(dx > 0 && dy > 0 && x >= 0 && y >= 0 && x < MAP_SIZE_X && y < MAP_SIZE_Y)
            {
                pushBId(&map[x + y * MAP_SIZE_X], bullet->id);
            }
        }
    }
}

void clearMap(MapCell *map, int size)
{
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < MAX_UNITS_IN_CELL; j++)
        {
            map[i].units[j] = -1; 
            map[i].bullets[j] = -1; 
        }
        map[i].cur_units = 0; 
        map[i].cur_bullets = 0; 
    }
}


