#include "bullet.h"



void spawnBullet(Bullet **bullets, int size, float x, float y, float dir_x, float dir_y, float velocity, float max_range, int team_id, int damage)
{
    for(int i = 0; i < size; i++)
    {
        if(bullets[i] == NULL)
        {
            bullets[i] = (Bullet *)malloc(sizeof(Bullet));
            bullets[i]->position.x = x;
            bullets[i]->position.y = y;
            bullets[i]->startPosition.x = x;
            bullets[i]->startPosition.y = y;
            bullets[i]->direction.x = dir_x;
            bullets[i]->direction.y = dir_y;
            bullets[i]->width = 4;
            bullets[i]->height = 4;
            bullets[i]->velocity = velocity;
            bullets[i]->max_range = max_range;
            bullets[i]->team_id = team_id;
            bullets[i]->damage = damage;
            bullets[i]->id = i;
            bullets[i]->shouldBeDestroyed = 0;
            break;
        }
    }
}

void destroyOldBullets(Bullet **bullets, int size)
{
    for(int i = 0; i < size; i++)
    {
        if(bullets[i] != NULL && bullets[i]->shouldBeDestroyed == 1)
        {
            free(bullets[i]);
            bullets[i] = NULL;
        }
    }
}

void updateBullet(Bullet *bullet, Bullet **bullets, int size, Uint64 deltaTime)
{
    float dx = bullet->position.x - bullet->startPosition.x;
    float dy = bullet->position.y - bullet->startPosition.y;
    float length = sqrt(dx * dx + dy * dy);
    bullet->position.x += bullet->velocity * bullet->direction.x * deltaTime;
    bullet->position.y += bullet->velocity * bullet->direction.y * deltaTime;
    if(length > bullet->max_range)
    {
        bullet->shouldBeDestroyed = 1;
    }
}

void updateBullets(Bullet **bullets, int size, Uint64 deltaTime)
{
    for(int i = 0; i < size; i++)
    {
        if(bullets[i])
        {
            updateBullet(bullets[i], bullets, size, deltaTime);
        }
    }
}

void drawBullet(Bullet *bullet)
{
    bullet->renderQuad.x = bullet->position.x - bullet->width / 2 - camera.x;
    bullet->renderQuad.y = bullet->position.y - bullet->height / 2 - camera.y;
    bullet->renderQuad.w = bullet->width;
    bullet->renderQuad.h = bullet->height;
    SDL_SetRenderDrawColor(renderer, 133, 133, 133, 255);    
    SDL_RenderFillRect(renderer, &bullet->renderQuad);
}

void drawBullets(Bullet **bullets, int size)
{
    for(int i = 0; i < size; i++)
    {
        if(bullets[i] != NULL)
        {
            drawBullet(bullets[i]);
        }
    }
}

void deInitBullets(Bullet **bullets, int size)
{
    for(int i = 0; i < size; i++)
    {
        if(bullets[i] != NULL)
        {
            free(bullets[i]);
            bullets[i] = NULL;
        }
    }
}

void destroyBullet(Bullet **bullets, int size, int id)
{
    if(bullets[id] != NULL)
    {
        free(bullets[id]);
        bullets[id] = NULL;
    }
}
