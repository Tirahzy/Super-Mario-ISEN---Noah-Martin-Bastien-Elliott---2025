#include "../fonction.h"

//---------------------------------------------------------
// Affichage du score / vies

void afficherScore(SDL_Renderer *renderer, ScoreJeu *scoreJeu, TTF_Font *police)
{
    char texte[64];
    sprintf(texte, "Score : %d", scoreJeu->score);

    SDL_Color couleur = {255, 255, 255};
    SDL_Surface *surfaceTexte = TTF_RenderText_Solid(police, texte, couleur);
    if (!surfaceTexte)
    {
        printf("Erreur TTF_RenderText_Solid\n");
        return;
    }

    SDL_Texture *textureTexte = SDL_CreateTextureFromSurface(renderer, surfaceTexte);
    if (!textureTexte)
    {
        printf("Erreur SDL_CreateTextureFromSurface\n");
        SDL_FreeSurface(surfaceTexte);
        return;
    }

    SDL_Rect dest = {10, 10, surfaceTexte->w, surfaceTexte->h};
    SDL_RenderCopy(renderer, textureTexte, NULL, &dest);

    SDL_FreeSurface(surfaceTexte);
    SDL_DestroyTexture(textureTexte);
}

void afficherVies(SDL_Renderer *renderer, ScoreJeu *scoreJeu, TexturesJeu textures)
{
    int taille = 30;
    int marge = 10;
    SDL_Rect rect;
    rect.y = 10;
    rect.w = taille;
    rect.h = taille;

    // Aligné à droite
    rect.x = LONGUEUR_FENETRE - scoreJeu->vies * (taille + marge);

    for (int i = 0; i < scoreJeu->vies; i++)
    {
        SDL_RenderCopy(renderer, textures.vie, NULL, &rect);
        rect.x += taille + marge;
    }
}