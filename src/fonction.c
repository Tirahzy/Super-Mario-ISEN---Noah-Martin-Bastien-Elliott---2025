#include "fonction.h"

SDL_Window *creerFenetre()
{
    SDL_Window *nv = NULL;

    nv = SDL_CreateWindow("Jeu", 0, 0, 1920, 1080, SDL_WINDOW_SHOWN);

    if (nv == NULL)
        return;
}