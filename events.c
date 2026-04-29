#include "events.h"
SDL_Event ev;

Mouse mouse = {.max_units = BUFFER_SIZE};
SelectMode selectMode = {0, 0, 0, 0};
Unit *selectedUnits[BUFFER_SIZE];

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
    for(int i = 0; i < mouse.max_units; i++)
    {
        mouse.selectedUnits[i] = NULL;
    }
}

void selectUnits(int sx, int sy, int ex, int ey, Unit **units, const int max_units)
{
    // clear selected units
    for(int i = 0; i < max_units; i++)
    {
        if(units[i] != NULL)
        {
            units[i]->isSelected = 0;
            mouse.selectedUnits[i] = NULL;
        }
    }
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
    for(int i = 0; i < max_units; i++)
    {
        if(units[i] != NULL)
        {
            if(units[i]->position.x > sx && units[i]->position.y > sy && units[i]->position.x < ex && units[i]->position.y < ey)
            {
                printf("unitX: %d, unitY: %d, camX: %d, camY: %d, sx: %d, sy: %d, ex: %d, ey: %d\n", units[i]->position.x, units[i]->position.y, camera.x, camera.y, sx, sy, ex, ey);
                units[i]->isSelected = 1;
                for(int j = 0; j < max_units; j++)
                {
                    if(mouse.selectedUnits[j] == NULL)
                    {
                        mouse.countSelectedUnits++;
                        mouse.selectedUnits[j] = units[i];
                        break;
                    }
                }
            }
        }
    }
}

void moveUnits(Unit **units, int max_size, int x, int y)
{
    x += camera.x;
    y += camera.y;
    for(int i = 0; i < max_size; i++)
    {
        if(mouse.selectedUnits[i])
        {
            mouse.selectedUnits[i]->target.x = x;
            mouse.selectedUnits[i]->target.y = y;
            mouse.selectedUnits[i]->action = MOVING;
        }
    }
}

void processMouse()
{
    
    if(mouse.mode == STANDARD)
    {
        if(ev.type == SDL_MOUSEBUTTONDOWN)
        {
            int x, y;
            SDL_GetMouseState(&x, &y);
            if(ev.button.button == SDL_BUTTON_LEFT)
            {
                mouse.mode = SELECTING;
            }
            if(ev.button.button == SDL_BUTTON_RIGHT)
            {
                moveUnits(mouse.selectedUnits, mouse.max_units, x, y);
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
            selectUnits(selectMode.startX, selectMode.startY, selectMode.endX, selectMode.endY, units, mouse.max_units);
            mouse.isHold = 0;
            mouse.mode = STANDARD;
        }
    }
}

void processKeyboard()
{
    const Uint8* numkeys = SDL_GetKeyboardState(NULL);
    if(numkeys[SDL_SCANCODE_W])
    {
        camera.y -= CAMERA_SPEED;
    }
    if(numkeys[SDL_SCANCODE_S])
    {
        camera.y += CAMERA_SPEED;
    }
    if(numkeys[SDL_SCANCODE_A])
    {
        camera.x -= CAMERA_SPEED;
    }
    if(numkeys[SDL_SCANCODE_D])
    {
        camera.x += CAMERA_SPEED;
    }
}

void processEvents()
{
    SDL_PollEvent(&ev);
    if(ev.type == SDL_QUIT){
        running = 0;
    }

    processMouse();
    processKeyboard();    
}