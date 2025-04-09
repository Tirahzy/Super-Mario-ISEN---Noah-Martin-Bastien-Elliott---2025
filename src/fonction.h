#ifndef FONCTION_H
#define FONCTION_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LONGUEUR_FENETRE 960
#define LARGEUR_FENETRE 540
#define SOL 390
#define GRAVITE 0.5f
#define FORCE_SAUT -12.0f
#define VITESSE_DEPLACEMENT 5

#define BLOC_SIZE 50
#define MAP_LARGEUR 150
#define MAP_HAUTEUR 12

extern int map[MAP_HAUTEUR][MAP_LARGEUR];

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

void initialiserMap();
void dessinerMap(SDL_Renderer *renderer, int cameraX);

SDL_Texture *chargerTextureBMP(SDL_Renderer *renderer, const char *chemin);

SDL_bool detecterCollision(SDL_Rect joueur);

#endif