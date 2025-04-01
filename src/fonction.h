#ifndef FONCTION_H
#define FONCTION_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int LONGUEUR_FENETRE;
const int LARGEUR_FENETRE;

SDL_Window* creerFenetre();
SDL_Renderer* creerRenderer(SDL_Window* fenetre);
void dessinerCarre(SDL_Renderer* renderer, SDL_Rect carre);
void gererEvenements(SDL_bool* continuer, SDL_Rect* carre);

#endif