#ifndef MAP_h_
#define MAP_h_

typedef struct 
{
    SDL_Rect rect;
    SDL_Rect renderRect;
    SDL_Color color;
    SDL_Texture *texture;
}
MapCell;

void drawMap(SDL_Renderer *renderer, const SDL_Rect camera);

#endif