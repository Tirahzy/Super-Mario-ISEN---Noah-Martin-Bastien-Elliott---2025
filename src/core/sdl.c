#include "../fonction.h"

//---------------------------------------------------------
// Fonction initalisation basique de SDL

SDL_Window *creerFenetre(char nom[])
{
    SDL_Window *fenetre = SDL_CreateWindow(nom, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                           LONGUEUR_FENETRE, LARGEUR_FENETRE, SDL_WINDOW_SHOWN);
    if (fenetre == NULL)
    {
        printf("Erreur SDL_CreateWindow\n");
        SDL_Quit();
        exit(1);
    }
    return fenetre;
}

SDL_Renderer *creerRenderer(SDL_Window *fenetre)
{
    SDL_Renderer *renderer = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        printf("Erreur SDL_CreateRenderer\n");
        SDL_DestroyWindow(fenetre);
        SDL_Quit();
        exit(1);
    }
    return renderer;
}

void dessinerCarre(SDL_Renderer *renderer, SDL_Rect carre)
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &carre);
}