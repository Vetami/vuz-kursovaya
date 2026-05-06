#ifndef UNIT_h_
#define UNIT_h_

#include <math.h>
#include <pthread.h>
#include <SDL2/SDL.h>
#include "defines.h"
#include "structs.h"
#include "player.h"
#include "bullet.h"

extern SDL_Rect camera;
extern SDL_Renderer *renderer;
extern MapCell c_map[MAP_SIZE_Y * MAP_SIZE_X];


typedef enum {
    Archer,
    Swordman,
    INFANTRY_MAN
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
    int attackCooldown;
    int currentAttackCooldown;
    const char *faction;
    float attackRange;
    float accuracy;
    float bulletSpeed;
    int damage;
    int hp;
    int unit_id;
    int team_id;
    int player_id;
    int isSelected;
} Unit;



void drawUnit(Unit *unit);
void drawUnits(Unit **units, const int size);
void updateUnit(Unit *unit, Uint64 deltaTime);
void updateUnits(Unit **units, const int size, Uint64 deltaTime);
void updateUnitsMT(Unit **units, const int size, Uint64 deltaTime);
void spawnUnit(Unit **unitsArray, const int size, UnitType type, const char *faction, Player player, const int x, const int y);
void deinitUnits(Unit **unitsArray, const int size);
void destroyUnit(Unit **unitsArray, const int size, const int id);
int checkAttack(Unit *unit, MapCell *map);
void unitAttack(Unit *attacker, Unit *other);
void getHit(Unit *unit, MapCell *map);
// int checkAllForAttack(Unit *attacker, Unit **units, int size);
int checkDead(Unit *unit);
void destroyOldUnits(Unit **units, const int size);

#endif