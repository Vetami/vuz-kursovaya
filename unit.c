#include "unit.h"

extern Unit *units[BUFFER_SIZE];

void drawUnit(Unit *unit)
{
    unit->renderQuad.x = unit->position.x - unit->width / 2 - camera.x;
    unit->renderQuad.y = unit->position.y - unit->height / 2 - camera.y;
    unit->renderQuad.w = unit->width;
    unit->renderQuad.h = unit->height;
    if(unit->isSelected)
    {
        unit->selectQuad.x = unit->position.x - unit->width / 2 - camera.x - 2;
        unit->selectQuad.y = unit->position.y - unit->height / 2 - camera.y - 2;
        unit->selectQuad.w = unit->width + 4;
        unit->selectQuad.h = unit->height + 4;
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &unit->selectQuad);
    }
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &unit->renderQuad);
}

void drawUnits(Unit **units, const int size)
{
    for(int i = 0; i < size; i++)
    {
        if(units[i] != NULL)
        {
            drawUnit(units[i]);
        }
    }
}

void updateUnit(Unit *unit, Uint64 deltaTime)
{
    int sx = unit->position.x;
    int sy = unit->position.y;

    int tgx = unit->target.x;
    int tgy = unit->target.y;

    int dx = tgx - sx;
    int dy = tgy - sy; // Results a vector vec2(dx, dy)

    float length = sqrt(dx*dx + dy*dy);
    if(length != 0)
    {
        unit->vecToTarget.x = dx / length;
        unit->vecToTarget.y = dy / length;
    } else {
        unit->vecToTarget.x = 0;
        unit->vecToTarget.y = 0;
    }
    if(unit->action == MOVING)
    {
        unit->position.x += unit->velocity * unit->vecToTarget.x * deltaTime;
        unit->position.y += unit->velocity * unit->vecToTarget.y * deltaTime;
        // printf("ux: %d, uy: %d, tx: %d, ty: %d, dx: %d, dy: %d\n",unit->position.x, unit->position.y, unit->target.x, unit->target.y, dx, dy);
        // if(sx < tgx && sy < tgy && sx + unit->width > tgx && unit->height + sy > tgy)
        // {
            // unit->action = STANDING;
        // }
        if(sx == tgx && sy == tgy)
        {
            unit->action = STANDING;
        }
    }
}

void updateUnits(Unit **units, const int size, Uint64 deltaTime)
{
    for(int i = 0; i < size; i++)
    {
        if(units[i] != NULL)
        {
            updateUnit(units[i], deltaTime);
        }
    }
}

void spawnUnit(Unit **unitsArray, const int size, UnitType type, const char *faction, const int player_id, const int x, const int y)
{
    for(int i = 0; i < size; i++)
    {
        if(unitsArray[i] == NULL)
        {
            unitsArray[i] = (Unit*)malloc(sizeof(Unit));
            unitsArray[i]->player_id = player_id;
            unitsArray[i]->faction = faction;
            unitsArray[i]->position.x = x;
            unitsArray[i]->position.y = y;
            unitsArray[i]->type = type;
            unitsArray[i]->velocity = 0.5f;
            unitsArray[i]->action = STANDING;
            unitsArray[i]->width = 10;
            unitsArray[i]->height = 10;
            unitsArray[i]->isSelected = 0;
            // todo spawn logic
            // if(unitsArray[i]->type == )
            // {
                // do something
            // }
            break;
        }
    }
}