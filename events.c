#include "events.h"
SDL_Event ev;

Mouse mouse = {.max_units = BUFFER_SIZE, .selectedUnits = {-1}};
SelectMode selectMode = {0, 0, 0, 0};
extern MapCell c_map[MAP_SIZE_X * MAP_SIZE_Y];
// Unit *selectedUnits[BUFFER_SIZE] = {-1};

void drawSelectSquare()
{
    if(mouse.mode == SELECTING)
    {
        selectMode.rect.x = selectMode.startX;
        selectMode.rect.y = selectMode.startY;
        selectMode.rect.w = selectMode.endX - selectMode.startX;
        selectMode.rect.h = selectMode.endY - selectMode.startY;
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &selectMode.rect);
    }
}

void initMouse()
{
    mouse.mode = STANDARD;
    mouse.isHold = 0;
    mouse.countSelectedUnits = 0;
}

void selectUnits(int sx, int sy, int ex, int ey, Unit **units, const int max_units, int player_id, MapCell *map, int cell_width)
{
    sx += camera.x;
    sy += camera.y;
    ex += camera.x;
    ey += camera.y;
    
    if(ex < sx)
    {
        int tx = ex;
        ex = sx;
        sx = tx;
    }
    if(ey < sy)
    {
        int ty = ey;
        ey = sy;
        sy = ty;
    }
    int sgx = sx;
    int sgy = sy;
    int egx = ex;
    int egy = ey;
    
    if(sy < 0)
    {
        sy = 0;
    }
    if(sx < 0)
    {
        sx = 0;
    }

    sx /= cell_width;
    sy /= cell_width;
    ex /= cell_width;
    ey /= cell_width;

    int dx = ex - sx;
    int dy = ey - sy;

    int offset = 3;
    
    for(int i = sy; i <= ey; i++)
    {
        for(int j = sx; j <= ex; j++)
        {
            int id = j + i * MAP_SIZE_X;
            if(id >= 0 && id < MAP_SIZE_X * MAP_SIZE_Y)
            {
                for(int k = 0; k < map[id].cur_units; k++)
                {
                    int uid = map[id].units[k];
                    if(units[uid]->position.x > sgx && units[uid]->position.y > sgy && units[uid]->position.x < egx && units[uid]->position.y < egy && units[uid]->player_id == player_id)
                    {
                        units[uid]->isSelected = 1;
                        for(int l = 0; l < max_units; l++)
                        {
                            if(mouse.selectedUnits[l] == -1)
                            {
                                mouse.countSelectedUnits++;
                                mouse.selectedUnits[l] = units[uid]->unit_id;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}

int selectUnit(int sx, int sy, Unit **units, const int max_units, int player_id, MapCell *map, int cell_width)
{
    sx += camera.x;
    sy += camera.y;
    int sgx = sx;
    int sgy = sy;
    if(sy < 0)
    {
        sy = 0;
    }
    if(sx < 0)
    {
        sx = 0;
    }

    sx /= cell_width;
    sy /= cell_width;


    int offset = 3;
    
    
    int id = sx + sy * MAP_SIZE_X;
    if(id >= 0 && id < MAP_SIZE_X * MAP_SIZE_Y)
    {
        for(int k = 0; k < map[id].cur_units; k++)
        {
            int uid = map[id].units[k];
            if(units[uid]->position.x - offset <= sgx && units[uid]->position.y - offset <= sgy && units[uid]->position.x + units[uid]->width + offset >= sgx && units[uid]->position.y + units[uid]->height + offset >= sgy && units[uid]->player_id == player_id)
            {
                units[uid]->isSelected = 1;
                for(int l = 0; l < max_units; l++)
                {
                    if(mouse.selectedUnits[l] == -1)
                    {
                        mouse.countSelectedUnits++;
                        mouse.selectedUnits[l] = units[uid]->unit_id;
                        break;
                    }
                }
            }
        }
    }
    return 0;
}

void orderUnits(int *units_id, int max_size, int x, int y, UnitAction order)
{
    x += camera.x;
    y += camera.y;
    for(int i = 0; i < max_size; i++)
    {
        if(mouse.selectedUnits[i] >= 0)
        {
            int uid = mouse.selectedUnits[i];
            if(units[uid])
            {
                units[uid]->target.x = x;
                units[uid]->target.y = y;
                units[uid]->action = order;
                // printf("i: %d inner: %d\n", i, mouse.selectedUnits[i]);
            }
            // printf("uid: %d\n", uid);
        }
    }
}

void clearSelectedUnits()
{
    for(int i = 0; i < mouse.max_units; i++)
    {
        mouse.selectedUnits[i] = -1;
    }
    for(int i = 0; i < BUFFER_SIZE; i++)
    {
        if(units[i] != NULL)
        {
            units[i]->isSelected = 0;
        }
    }
    mouse.countSelectedUnits = 0;
}

void processMouse(SDL_Event ev)
{
    
    if(mouse.mode == STANDARD)
    {
        if(ev.type == SDL_MOUSEBUTTONDOWN)
        {
            int x, y;
            SDL_GetMouseState(&x, &y);
            if(ev.button.button == SDL_BUTTON_LEFT)
            {
                clearSelectedUnits();
                if(!selectUnit(x, y, units, BUFFER_SIZE, player.id, c_map, CELL_SIZE))
                {
                    mouse.mode = SELECTING;
                }
            }
            if(ev.button.button == SDL_BUTTON_RIGHT)
            {
                orderUnits(mouse.selectedUnits, mouse.max_units, x, y, MOVING);
                // printf("tgx: %d, tgy: %d\n", x, y);
            }
        }
    }

    if(mouse.mode == SELECTING && ev.button.button == SDL_BUTTON_LEFT)
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        if(mouse.isHold == 0)
        {
            mouse.isHold = 1;
            selectMode.startX = x;
            selectMode.startY = y;
        }
        if(mouse.isHold == 1)
        {
            selectMode.endX = x;
            selectMode.endY = y;
        }
        if(mouse.isHold == 1 && ev.type == SDL_MOUSEBUTTONUP)
        {
            selectUnits(selectMode.startX, selectMode.startY, selectMode.endX, selectMode.endY, units, mouse.max_units, player.id, c_map, CELL_SIZE);
            mouse.isHold = 0;
            mouse.mode = STANDARD;
        }
    }
}



void processKeyboard(Uint64 deltaTime)
{
    const Uint8* numkeys = SDL_GetKeyboardState(NULL);
    if(numkeys[SDL_SCANCODE_W])
    {
        camera.y -= CAMERA_SPEED * deltaTime;
    }
    if(numkeys[SDL_SCANCODE_S])
    {
        camera.y += CAMERA_SPEED * deltaTime;
    }
    if(numkeys[SDL_SCANCODE_A])
    {
        camera.x -= CAMERA_SPEED * deltaTime;
    }
    if(numkeys[SDL_SCANCODE_D])
    {
        camera.x += CAMERA_SPEED * deltaTime;
    }
}

void processEvents(Uint64 deltaTime)
{
    while(SDL_PollEvent(&ev))
    {
        if(ev.type == SDL_QUIT){
            running = 0;
        }
        processMouse(ev);
    }
    
    

    processKeyboard(deltaTime);    
}