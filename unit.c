#include "unit.h"

Unit *units[BUFFER_SIZE];

void drawUnit(Unit *unit)
{
    unit->renderQuad.x = unit->position.x - camera.x;
    unit->renderQuad.y = unit->position.y - camera.y;
    unit->renderQuad.w = unit->width;
    unit->renderQuad.h = unit->height;
    if(unit->isSelected)
    {
        unit->selectQuad.x = unit->position.x - camera.x - 2;
        unit->selectQuad.y = unit->position.y - camera.y - 2;
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

void updateUnit(Unit *unit)
{
    int sx = unit->position.x;
    int sy = unit->position.y;

    int tgx = unit->target.x;
    int tgy = unit->target.y;

    int dx = tgx - sx;
    int dy = tgy - sy; // Results a vector vec2(dx, dy)

    float fi;
    if(dx != 0)
    {
        fi = atan2((double)dy, (double)dx);
    } else if(dy > 0) {
        fi = M_PI / 2;
    } else if(dy < 0) {
        fi = M_PI + M_PI / 2;
    }
    unit->angle = fi;
    if(unit->action == MOVING)
    {
        unit->position.x += unit->velocity * cos(unit->angle);
        unit->position.y += unit->velocity * sin(unit->angle);
        // printf("fi: %lf, ux: %d, uy: %d, tx: %d, ty: %d\n",fi, unit->position.x, unit->position.y, unit->target.x, unit->target.y);
        if(sx - unit->width / 2 < tgx && sy - unit->height / 2 < tgy && sx + unit->width > tgx && unit->height + sy > tgy)
        {
            unit->action = STANDING;
        }
    }
}

void updateUnits(Unit **units, const int size)
{
    for(int i = 0; i < size; i++)
    {
        if(units[i] != NULL)
        {
            updateUnit(units[i]);
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
            unitsArray[i]->velocity = 3.0f;
            unitsArray[i]->action = STANDING;
            unitsArray[i]->width = 10;
            unitsArray[i]->height = 10;
            // todo spawn logic
            // if(unitsArray[i]->type == )
            // {
                // do something
            // }
            break;
        }
    }
}