#ifndef MAP_h_
#define MAP_h_

#include "defines.h"
#include "unit.h"
#include "structs.h"



void pushId(MapCell *cell, int id);
void initMap(MapCell *c_map);
void drawMap(SDL_Renderer *renderer, const SDL_Rect camera, MapCell *c_map);
void updateMap(Unit **units, int size, MapCell *map, int m_size, Bullet **bullets, int b_size);
void clearMap(MapCell *map, int size);
void deInitMap(MapCell *c_map);

#endif