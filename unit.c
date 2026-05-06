#include "unit.h"

extern Unit *units[BUFFER_SIZE];
static int unit_counter = 0;

void spawnUnit(Unit **unitsArray, const int size, UnitType type, const char *faction, Player player, const int x, const int y)
{
    for(int i = 0; i < size; i++)
    {
        if(unitsArray[i] == NULL)
        {
            unitsArray[i] = (Unit*)malloc(sizeof(Unit));
            unitsArray[i]->player_id = player.id;
            unitsArray[i]->team_id = player.team_id;
            unitsArray[i]->unit_id = i;
            unitsArray[i]->faction = faction;
            unitsArray[i]->position.x = x;
            unitsArray[i]->position.y = y;
            unitsArray[i]->type = type;
            unitsArray[i]->action = STANDING;
            if(unitsArray[i]->type == INFANTRY_MAN)
            {
                unitsArray[i]->velocity = 0.1f;
                unitsArray[i]->width = 10;
                unitsArray[i]->height = 10;
                unitsArray[i]->attackCooldown = 10000;
                unitsArray[i]->currentAttackCooldown = 0;
                unitsArray[i]->bulletSpeed = 0.1f;
                unitsArray[i]->isSelected = 0;
                unitsArray[i]->attackRange = 1080;
                unitsArray[i]->hp = 100;
                unitsArray[i]->damage = 1;
            }
            unit_counter++;
            break;
        }
    }
}

void destroyUnit(Unit **unitsArray, const int size, const int id)
{
    for(int i = 0; i < size; i++)
    {
        if(unitsArray[i] != NULL)
        {
            if(unitsArray[i]->unit_id == id)
            {
                free(unitsArray[i]);
                unitsArray[i] = NULL;
            }
        }
    }
}

void deinitUnits(Unit **unitsArray, const int size)
{
    for(int i = 0; i < size; i++)
    {
        if(unitsArray[i] != NULL)
        {
            free(unitsArray[i]);
            unitsArray[i] = NULL;
        }
    }
}


int checkAttack(Unit *unit, MapCell * map)
{
    float attackRange = unit->attackRange;
    int attackRangeCells = attackRange / CELL_SIZE;
    int x = (int)unit->position.x / CELL_SIZE;
    int y = (int)unit->position.y / CELL_SIZE;
    int cur_unit = -1;
    float min_range = 999999;
    for(int i = y - attackRangeCells; i <= y + attackRangeCells; i++)
    {
        for(int j = x - attackRangeCells; j <= x + attackRangeCells; j++)
        {  
            if(i >= 0 && i < MAP_SIZE_Y && j >= 0 && j < MAP_SIZE_X)
            {
                for(int k = 0; k < map[j + i * MAP_SIZE_X].cur_units; k++)
                {
                    if(units[map[j + i * MAP_SIZE_X].units[k]]->team_id != unit->team_id)
                    {
                        int other = map[j + i * MAP_SIZE_X].units[k];
                        float dx = unit->position.x - units[other]->position.x;
                        float dy = unit->position.y - units[other]->position.y;
                        float length = sqrt(dx * dx + dy * dy);
                        if(length < min_range)
                        {
                            min_range = length;
                            cur_unit = units[other]->unit_id;
                        }
                    }
                }
            }
        }
    }
    return cur_unit;
}


void unitAttack(Unit *attacker, Unit *other)
{
    if(attacker->currentAttackCooldown == 0)
    {
        vec2 direction;
        float dx = other->position.x - attacker->position.x;
        float dy = other->position.y - attacker->position.y;
        float length = sqrt(dx * dx + dy * dy);
        if(length != 0)
        {
            direction.x = dx / length;
            direction.y = dy / length;
        }
            spawnBullet(bullets, BULLET_BUFFER_SIZE, attacker->position.x, attacker->position.y, direction.x, direction.y, attacker->bulletSpeed, attacker->attackRange, attacker->team_id, attacker->damage);
            attacker->currentAttackCooldown = attacker->attackCooldown;
    }
}

void getHit(Unit *unit, MapCell *map)
{
    int x = unit->position.x / CELL_SIZE;
    int y = unit->position.y / CELL_SIZE;
    for(int i = y - 1; i <= y + 1; i++)
    {
        for(int j = x - 1; j <= x + 1; j++)
        {
            if(i >= 0 && i < MAP_SIZE_Y && j >= 0 && j < MAP_SIZE_X)
            {
                if(map[j + i * MAP_SIZE_X].cur_bullets > 0)
                {
                    for(int k = 0; k < map[j + i * MAP_SIZE_X].cur_bullets; k++)
                    {
                        if(unit->team_id != bullets[map[j+i*MAP_SIZE_X].bullets[k]]->team_id && SDL_HasIntersection(&unit->renderQuad, &bullets[map[j+i*MAP_SIZE_X].bullets[k]]->renderQuad))
                        {
                            unit->hp -= bullets[map[j+i*MAP_SIZE_X].bullets[k]]->damage;
                            bullets[map[j+i*MAP_SIZE_X].bullets[k]]->shouldBeDestroyed = 1;
                        }
                    }
                }
            }
        }
    }
}

int checkDead(Unit *unit)
{
    if(unit->hp <= 0)
    {
        destroyUnit(units, BUFFER_SIZE, unit->unit_id);
        return 1;
    }
    return 0;
}

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
    if(!strcmp(unit->faction, "blue"))
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);    
    }
    else if(!strcmp(unit->faction, "red"))
    {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);    
    }
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

void destroyOldUnits(Unit **units, const int size)
{
    for(int i = 0; i < size; i++)
    {
        if(units[i] != NULL && units[i]->hp <= 0)
        {
            free(units[i]);
            units[i] = NULL;
        }
    }
}

void updateUnit(Unit *unit, Uint64 deltaTime)
{
    if(unit->currentAttackCooldown > 0)
    {
        unit->currentAttackCooldown -= deltaTime;
    } else if(unit->currentAttackCooldown < 0)
    {
        unit->currentAttackCooldown = 0;
    }
    getHit(unit, c_map);
    if(unit->action == MOVING)
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
        unit->position.x += unit->velocity * unit->vecToTarget.x * deltaTime;
        unit->position.y += unit->velocity * unit->vecToTarget.y * deltaTime;
        if(sx == tgx && sy == tgy)
        {
            unit->action = STANDING;
        }
    }
    if(unit->action == STANDING)
    {
        int other = checkAttack(unit, c_map);
        if(other >= 0)
        {
            if(units[other] != NULL)
            {
                unitAttack(unit, units[other]);
            }
        }
    }
}

void updateUnitsSE(int start, int end, Uint64 deltaTime)
{
    for(int i = start; i < end; i++)
    {
        if(units[i] != NULL)
        {
            updateUnit(units[i], deltaTime);
        }
    }
}

void *updateUnitsWrapper(void *arg)
{
    ThreadData *data = (ThreadData *) arg;
    updateUnitsSE(data->startUnit, data->endUnit, data->deltaTime);
    return NULL;
}

void updateUnitsMT(Unit **units, const int size, Uint64 deltaTime)
{
    pthread_t thread[MAX_THREADS];
    int unitsPerThread = size / MAX_THREADS;
    int unitsRemain = size % MAX_THREADS;
    ThreadData threadData[MAX_THREADS];
    for(int i = 0; i < MAX_THREADS; i++)
    {
        threadData[i].deltaTime = deltaTime;
        threadData[i].startUnit =  unitsPerThread * i;
        threadData[i].endUnit =  unitsPerThread * i + unitsPerThread;
        pthread_create(&thread[i], NULL, updateUnitsWrapper, &threadData[i]);
    }
    for(int i = 0; i < MAX_THREADS; i++)
    {
        pthread_join(thread[i], NULL);
    }
    if(unitsRemain > 0)
    {
        threadData[0].deltaTime = deltaTime;
        threadData[0].startUnit =  size - unitsRemain;
        threadData[0].endUnit =  size - 1;
        pthread_create(&thread[0], NULL, updateUnitsWrapper, &threadData[0]);
        pthread_join(thread[0], NULL);
    }
}

void updateUnits(Unit **units, const int size, Uint64 deltaTime)
{
    for(int i = 0; i < size; i++)
    {
        if(units[i])
        {
            updateUnit(units[i], deltaTime);
        }
    }
}



// int checkAllForAttack(Unit *attacker, Unit **units, int size)
// {
//     for(int j = 0; j < size; j++)
//     {
//         if(units[j] != NULL)
//         {
//             if(checkForAttack(attacker, units[j]))
//             {
//                 unitAttack(attacker, units[j]);
//             }
//         }    
//     }
// }
