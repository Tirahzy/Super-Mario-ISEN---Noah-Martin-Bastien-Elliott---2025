#ifndef FONCTION_H
#define FONCTION_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int LONGUEUR_FENETRE;
const int LARGEUR_FENETRE;
const int SOL;
const float GRAVITE;
const float FORCE_SAUT;
const int VITESSE_DEPLACEMENT;

typedef struct
{
    SDL_bool gauche;
    SDL_bool droite;
    SDL_bool saut;
} Touches;

SDL_Window *creerFenetre(char nom[]);
SDL_Renderer *creerRenderer(SDL_Window *fenetre);
void dessinerCarre(SDL_Renderer *renderer, SDL_Rect carre);
void gererEvenements(SDL_bool *continuer, SDL_Rect *carre, SDL_bool *enSaut, float *vitesseSaut, Touches *touches);

#endif