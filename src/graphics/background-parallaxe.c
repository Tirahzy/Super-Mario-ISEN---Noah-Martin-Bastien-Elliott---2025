#include "../fonction.h"

//---------------------------------------------------------
// Affichage background

void dessinerFondParallaxe(SDL_Renderer *renderer, SDL_Texture *texture, int cameraX)
{
    int vitesseParallaxe = 3;
    int offsetX = cameraX / vitesseParallaxe;

    SDL_Rect dst = {-offsetX, 0, LONGUEUR_FENETRE, LARGEUR_FENETRE};
    while (dst.x < LONGUEUR_FENETRE)
    {
        SDL_RenderCopy(renderer, texture, NULL, &dst);
        dst.x += LONGUEUR_FENETRE;
    }
}