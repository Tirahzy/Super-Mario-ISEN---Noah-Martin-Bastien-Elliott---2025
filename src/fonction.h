#ifndef FONCTION_H
#define FONCTION_H

#include <stdlib.h>
#include <SDL2/SDL.h>

typedef struct
{
    int x;
    int y;
    int w;
    int h;
} Mario;

SDL_Window *creerFenetre();

#endif