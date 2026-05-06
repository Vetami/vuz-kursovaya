#ifndef BULLET_H_
#define BULLET_H_

#include "structs.h"
#include "unit.h"
#include "SDL2/SDL.h"


typedef struct {
    vec2 startPosition;
    vec2 position;
    vec2 direction;
    float width;
    float height;
    float velocity;
    float max_range;
    int damage;
    int id;
    int team_id;
    int shouldBeDestroyed;
    SDL_Rect renderQuad;
} Bullet;

extern Bullet *bullets[BUFFER_SIZE * 4];

void destroyBullet(Bullet **bullets, int size, int id);
void spawnBullet(Bullet **bullets, int size, float x, float y, float dir_x, float dir_y, float velocity, float max_range, int team_id, int damage);
void updateBullet(Bullet *bullet, Bullet **bullets, int size, Uint64 deltaTime);
void updateBullets(Bullet **bullets, int size, Uint64 deltaTime);
void drawBullet(Bullet *bullet);
void drawBullets(Bullet **bullets, int size);
void destroyOldBullets(Bullet **bullets, int size);
void deInitBullets(Bullet **bullets, int size);

#endif