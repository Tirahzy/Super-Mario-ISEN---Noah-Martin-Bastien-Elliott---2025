#include "fonction.h"

SDL_Window* creerFenetre()
{
    SDL_Window *nv = SDL_CreateWindow("fntr", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        640, 480, SDL_WINDOW_SHOWN);
    if(nv == NULL)
    {
        printf("Erreur d'allocation !\n");
        exit(1);
    }
    return nv;
}