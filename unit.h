#ifndef UNIT_h_
#define UNIT_h_

#include <math.h>
#include <SDL2/SDL.h>
#include "defines.h"
#include "structs.h"

extern SDL_Rect camera;
extern SDL_Renderer *renderer;

typedef enum {
    Archer,
    Swordman
} UnitType;

typedef enum {
    STANDING,
    MOVING,
    ATTACKING
} UnitAction;

typedef struct {
    UnitType type;
    UnitAction action;
    SDL_Point target;
    vec2 position;
    vec2 vecToTarget; // normalized vec to target
    int width;
    int height;
    float angle;
    float velocity;
    SDL_Rect hitbox;
    SDL_Rect renderQuad;
    SDL_Rect selectQuad;
    SDL_Texture *texture;
    const char *faction;
    float range;
    float accuracy;
    int damage;
    int hp;
    int unit_id;
    int player_id;
    int isSelected;
} Unit;

void drawUnit(Unit *unit);
void drawUnits(Unit **units, const int size);
void updateUnit(Unit *unit, Uint64 deltaTime);
void updateUnits(Unit **units, const int size, Uint64 deltaTime);
void spawnUnit(Unit **unitsArray, const int size, UnitType type, const char *faction, const int player_id, const int x, const int y);


#endif