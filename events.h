#ifndef EVENTS_h_
#define EVENTS_h_
#include <SDL2/SDL.h>
#include "defines.h"
#include "unit.h"
#include "player.h"
#include "map.h"

extern SDL_Rect camera;
extern Uint8 running;
extern SDL_Renderer *renderer;
extern Unit *units[BUFFER_SIZE];
extern Player player;

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
    int selectedUnits[BUFFER_SIZE];
    const int max_units;
    int countSelectedUnits;
    int isHold;
} Mouse;

void drawSelectSquare();
void processEvents(Uint64 deltaTime);
void initMouse();
void clearSelectedUnits();
int selectUnit(int sx, int sy, Unit **units, const int max_units, int player_id, MapCell *map, int cell_width);
void selectUnits(int sx, int sy, int ex, int ey, Unit **units, const int max_units, int player_id, MapCell *map, int cell_width);
void orderUnits(int *units_id, int max_size, int x, int y, UnitAction order);

#endif