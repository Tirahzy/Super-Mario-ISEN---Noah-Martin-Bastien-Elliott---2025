#include "../fonction.h"

void afficherTableauScores(SDL_Renderer *renderer, TTF_Font *police)
{
    Sauvegarde sauvegardes[MAX_SAUVEGARDES];
    int nb = chargerToutesLesSauvegardes(sauvegardes, MAX_SAUVEGARDES);

    if (nb == 0)
        return;

    // Tri décroissant des scores
    for (int i = 0; i < nb - 1; i++)
    {
        for (int j = i + 1; j < nb; j++)
        {
            if (sauvegardes[j].scoreMax > sauvegardes[i].scoreMax)
            {
                Sauvegarde tmp = sauvegardes[i];
                sauvegardes[i] = sauvegardes[j];
                sauvegardes[j] = tmp;
            }
        }
    }

    int afficher = (nb < 10) ? nb : 10;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0.5); // fond semi-transparent
    SDL_Rect fond = {LONGUEUR_FENETRE / 2 - 250, 100, 500, 60 + afficher * 40};
    SDL_RenderFillRect(renderer, &fond);

    SDL_Color couleurTexte = {255, 255, 255};
    SDL_Surface *titre = TTF_RenderText_Solid(police, "CLASSEMENT DES SCORES", couleurTexte);
    SDL_Texture *titreTex = SDL_CreateTextureFromSurface(renderer, titre);
    SDL_Rect posTitre = {fond.x + (fond.w - titre->w) / 2, fond.y + 10, titre->w, titre->h};
    SDL_RenderCopy(renderer, titreTex, NULL, &posTitre);
    SDL_FreeSurface(titre);
    SDL_DestroyTexture(titreTex);

    for (int i = 0; i < afficher; i++)
    {
        char ligne[128];
        snprintf(ligne, sizeof(ligne), "%2d. %-20s  %5d pts", i + 1, sauvegardes[i].nom, sauvegardes[i].scoreMax);

        SDL_Surface *surface = TTF_RenderText_Solid(police, ligne, couleurTexte);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect pos = {
            fond.x + 30,
            fond.y + 50 + i * 35,
            surface->w,
            surface->h};
        SDL_RenderCopy(renderer, texture, NULL, &pos);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }

    SDL_RenderPresent(renderer);

    SDL_Event ev;
    int attendre = 1;
    while (attendre)
    {
        while (SDL_PollEvent(&ev))
        {
            if (ev.type == SDL_QUIT)
            {
                attendre = 0;
            }
            else if (ev.type == SDL_KEYDOWN || ev.type == SDL_MOUSEBUTTONDOWN)
            {
                attendre = 0;
            }
        }
        SDL_Delay(10);
    }
}