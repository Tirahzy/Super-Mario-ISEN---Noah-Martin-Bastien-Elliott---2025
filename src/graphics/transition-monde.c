#include "../fonction.h"

//---------------------------------------------------------
// Affichage des transitions entre les mondes

void afficherMonde2(SDL_Renderer *renderer, TTF_Font *police)
{
    SDL_Color couleur = {255, 255, 255};
    SDL_Surface *surfaceTexte = TTF_RenderText_Solid(police, "Monde 2", couleur);
    if (!surfaceTexte)
    {
        printf("Erreur TTF_RenderText_Solid\n");
        return;
    }

    SDL_Texture *textureTexte = SDL_CreateTextureFromSurface(renderer, surfaceTexte);
    SDL_FreeSurface(surfaceTexte);
    if (!textureTexte)
    {
        printf("Erreur SDL_CreateTextureFromSurface\n");
        return;
    }

    SDL_Rect rectTexte = {
        (LONGUEUR_FENETRE - surfaceTexte->w) / 2,
        (LARGEUR_FENETRE - surfaceTexte->h) / 2,
        surfaceTexte->w,
        surfaceTexte->h};

    int continuer = 1;
    SDL_Event event;

    while (continuer)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN)
            {
                continuer = 0;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, textureTexte, NULL, &rectTexte);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyTexture(textureTexte);
}

void afficherMonde3(SDL_Renderer *renderer, TTF_Font *police)
{
    SDL_Color couleur = {255, 255, 255};
    SDL_Surface *surfaceTexte = TTF_RenderText_Solid(police, "Monde 3", couleur);
    if (!surfaceTexte)
    {
        printf("Erreur TTF_RenderText_Solid\n");
        return;
    }

    SDL_Texture *textureTexte = SDL_CreateTextureFromSurface(renderer, surfaceTexte);
    SDL_FreeSurface(surfaceTexte);
    if (!textureTexte)
    {
        printf("Erreur SDL_CreateTextureFromSurface\n");
        return;
    }

    SDL_Rect rectTexte = {
        (LONGUEUR_FENETRE - surfaceTexte->w) / 2,
        (LARGEUR_FENETRE - surfaceTexte->h) / 2,
        surfaceTexte->w,
        surfaceTexte->h};

    int continuer = 1;
    SDL_Event event;

    while (continuer)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN)
            {
                continuer = 0;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, textureTexte, NULL, &rectTexte);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyTexture(textureTexte);
}

void afficherEcranFin(SDL_Renderer *renderer, TTF_Font *police)
{
    SDL_Color couleur = {255, 255, 255};
    SDL_Surface *surfaceTexte = TTF_RenderText_Solid(police, "Bien joue ! Jeu termine !", couleur);
    if (!surfaceTexte)
    {
        printf("Erreur TTF_RenderText_Solid\n");
        return;
    }

    SDL_Texture *textureTexte = SDL_CreateTextureFromSurface(renderer, surfaceTexte);
    SDL_FreeSurface(surfaceTexte);
    if (!textureTexte)
    {
        printf("Erreur SDL_CreateTextureFromSurface\n");
        return;
    }

    SDL_Rect rectTexte = {
        (LONGUEUR_FENETRE - surfaceTexte->w) / 2,
        (LARGEUR_FENETRE - surfaceTexte->h) / 2,
        surfaceTexte->w,
        surfaceTexte->h};

    int continuer = 1;
    SDL_Event event;

    while (continuer)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN)
            {
                continuer = 0;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, textureTexte, NULL, &rectTexte);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyTexture(textureTexte);
}