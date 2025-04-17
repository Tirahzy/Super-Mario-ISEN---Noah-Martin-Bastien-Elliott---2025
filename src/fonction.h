#ifndef FONCTION_H
#define FONCTION_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define LONGUEUR_FENETRE 960
#define LARGEUR_FENETRE 540

#define TAILLE_PERSO 30
#define SOL ((MAP_HAUTEUR - 3) * BLOC_SIZE - TAILLE_PERSO)
#define GRAVITE 0.5f
#define FORCE_SAUT -11.0f
#define VITESSE_DEPLACEMENT 5

#define BLOC_SIZE 30
#define MAP_LARGEUR 200
#define MAP_HAUTEUR 18

#define BLOC_PIECE 3
#define BLOC_ENNEMI 4
#define BLOC_QUESTION 5
#define BLOC_TUYAU 6

#define MAX_ENNEMIS 20
#define ENNEMI_LARGEUR 30
#define ENNEMI_HAUTEUR 30

#define MAX_EFFETS 20

// États du jeu
#define ETAT_MENU 0
#define ETAT_JEU 1
#define ETAT_GAME_OVER 2

extern int map[MAP_HAUTEUR][MAP_LARGEUR];

typedef struct
{
    SDL_bool gauche;
    SDL_bool droite;
    SDL_bool saut;
} Touches;

typedef struct {
    SDL_Rect rect;
    int vitesse;
    SDL_bool actif;
} Ennemi;

typedef struct {
    SDL_Rect rect;
    int duree;
    SDL_bool actif;
} Effet;

extern Ennemi ennemis[MAX_ENNEMIS];
extern Effet effets[MAX_EFFETS];

typedef struct {
    SDL_Rect rect;
    char* texte;
    SDL_bool hover;
} Bouton;

SDL_Window *creerFenetre(char nom[]);
SDL_Renderer *creerRenderer(SDL_Window *fenetre);

void dessinerCarre(SDL_Renderer *renderer, SDL_Rect carre);
void gererEvenements(SDL_bool *continuer, SDL_Rect *carre, SDL_bool *enSaut, float *vitesseSaut, Touches *touches);

void initialiserMap();
void initialiserEnnemis(char niveau[][MAP_LARGEUR]);
void dessinerMap(SDL_Renderer *renderer, int cameraX);

void initialiserEffets();
void ajouterEffetEcrasement(int x, int y);
void mettreAJourEffets();
void dessinerEffets(SDL_Renderer *renderer, int cameraX);

SDL_Texture *chargerTextureBMP(SDL_Renderer *renderer, const char *chemin);

SDL_bool detecterCollision(SDL_Rect joueur);
SDL_bool detecterCollisionEntreEnnemis(SDL_Rect ennemi, int indexEnnemi);
SDL_bool sauterSurEnnemi(SDL_Rect joueur, float vitesseSaut);

void afficherScore(SDL_Renderer *renderer, int nbPieces, TTF_Font *police);

void dessinerEnnemis(SDL_Renderer *renderer, int cameraX);
void mettreAJourEnnemis();
SDL_bool detecterCollisionEnnemi(SDL_Rect joueur);

void initialiserBoutons(Bouton boutons[], int nombreBoutons);
void dessinerBoutons(SDL_Renderer *renderer, Bouton boutons[], int nombreBoutons, TTF_Font *police);
int gererEvenementsMenu(SDL_bool *continuer, Bouton boutons[], int nombreBoutons);

#endif