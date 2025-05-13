// ======= fonction.h =======
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
#define NOMBRE_NIVEAUX 9

#define BLOC_SIZE 30
#define MAP_LARGEUR 200
#define MAP_HAUTEUR 18

#define BLOC_PIECE 3
#define BLOC_ENNEMI 4
#define BLOC_QUESTION 5
#define BLOC_TUYAU_HAUT_DROITE 6
#define BLOC_FIN 7
#define BLOC_TUYAU_HAUT_GAUCHE 8
#define BLOC_TUYAU_BAS_DROITE 9
#define BLOC_TUYAU_BAS_GAUCHE 10
#define BLOC_INCASSABLE 11

#define MAX_ENNEMIS 20
#define ENNEMI_LARGEUR 30
#define ENNEMI_HAUTEUR 30

#define MAX_EFFETS 20

#define ETAT_MENU 0
#define ETAT_JEU 1
#define ETAT_GAME_OVER 2
#define ETAT_NIVEAU_TERMINE 3

#define FPS_CIBLE 60
#define FRAME_TIME_MS (1000 / FPS_CIBLE)

extern int map[MAP_HAUTEUR][MAP_LARGEUR];

typedef struct {
    unsigned int dernierTemps;
    unsigned int frameActuelle;
    unsigned int frameCount;
    float fps;
    unsigned int dernierCalculFPS;
} GestionnaireFPS;

typedef struct {
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

typedef struct {
    SDL_Texture *perso;
    SDL_Texture *brique;
    SDL_Texture *piece;
    SDL_Texture *tuyau_haut_droite;
    SDL_Texture *tuyau_haut_gauche;
    SDL_Texture *tuyau_bas_droite;
    SDL_Texture *tuyau_bas_gauche;
    SDL_Texture *ennemi;
    SDL_Texture *questionBloc;
    SDL_Texture *sol;
    SDL_Texture *bloc_incassable;
    SDL_Texture *background;
} TexturesJeu;

extern Ennemi ennemis[MAX_ENNEMIS];
extern Effet effets[MAX_EFFETS];

typedef struct {
    SDL_Rect rect;
    char* texte;
    SDL_bool hover;
} Bouton;

SDL_Window *creerFenetre(char nom[]);
SDL_Renderer *creerRenderer(SDL_Window *fenetre);

void initialiserFPS(GestionnaireFPS *fps);
void limiterFPS(GestionnaireFPS *fps);
void calculerFPS(GestionnaireFPS *fps);
void afficherFPS(SDL_Renderer *renderer, GestionnaireFPS *fps, TTF_Font *police);

TexturesJeu chargerTextures(SDL_Renderer *renderer);
void libererTextures(TexturesJeu textures);

void dessinerCarre(SDL_Renderer *renderer, SDL_Rect carre);
void gererEvenements(SDL_bool *continuer, SDL_Rect *carre, SDL_bool *enSaut, float *vitesseSaut, Touches *touches);

void initialiserMap(int niveau);
void initialiserEnnemis(char niveau[][MAP_LARGEUR]);
void dessinerMap(SDL_Renderer *renderer, int cameraX, TexturesJeu textures);
void dessinerEnnemis(SDL_Renderer *renderer, int cameraX, TexturesJeu textures);

void initialiserEffets();
void ajouterEffetEcrasement(int x, int y);
void mettreAJourEffets();
void dessinerEffets(SDL_Renderer *renderer, int cameraX);

void dessinerTexture(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y, int w, int h, int cameraX);
SDL_Texture *chargerTextureBMP(SDL_Renderer *renderer, const char *chemin);

SDL_bool detecterCollision(SDL_Rect joueur);
SDL_bool detecterCollisionEntreEnnemis(SDL_Rect ennemi, int indexEnnemi);
SDL_bool sauterSurEnnemi(SDL_Rect joueur, float vitesseSaut);
SDL_bool detecterCollisionEnnemi(SDL_Rect joueur);
SDL_bool pointDansRect(int x, int y, SDL_Rect rect);

void afficherScore(SDL_Renderer *renderer, int nbPieces, TTF_Font *police);

void initialiserBoutons(Bouton boutons[], int nombreBoutons);
void dessinerBoutons(SDL_Renderer *renderer, Bouton boutons[], int nombreBoutons, TTF_Font *police);
int gererEvenementsMenu(SDL_bool *continuer, Bouton boutons[], int nombreBoutons);
SDL_bool finDeNiveau(SDL_Rect joueur);
int gererEvenementsNiveauTermine(SDL_bool *continuer, Bouton boutons[], int nombreBoutons);
void afficherEcranFin(SDL_Renderer *renderer, TTF_Font *police);
void afficherMonde2(SDL_Renderer *renderer, TTF_Font *police);
void dessinerFondRepete(SDL_Renderer *renderer, SDL_Texture *texture, int cameraX);


#endif
