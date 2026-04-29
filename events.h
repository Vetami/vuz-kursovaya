#ifndef EVENTS_h_
#define EVENTS_h_
#include <SDL2/SDL.h>
#include "defines.h"
#include "unit.h"

extern SDL_Rect camera;
extern Uint8 running;
extern SDL_Renderer *renderer;
extern Unit *units[BUFFER_SIZE];

typedef enum {
    STANDARD,
    SELECTING
} MouseMode;

typedef struct {
    int startX, startY;
    int endX, endY;
    SDL_Rect rect;
} SelectMode;

typedef struct {
    MouseMode mode;
    Unit *selectedUnits[BUFFER_SIZE];
    const int max_units;
    int countSelectedUnits;
    int isHold;
} Mouse;

void drawSelectSquare();
void processEvents(Uint64 deltaTime);
void initMouse();

#endif