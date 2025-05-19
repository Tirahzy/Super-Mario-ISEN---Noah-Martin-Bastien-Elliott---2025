#include "../fonction.h"

//---------------------------------------------------------
// Gestions des touches et des collisions (base du jeu)

void gererTouches(int *continuer, SDL_Rect *carre, int *enSaut, float *vitesseSaut, Touches *touches, SonsJeu sons)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            *continuer = 0;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_q:
                touches->gauche = 1;
                break;
            case SDLK_d:
                touches->droite = 1;
                break;
            case SDLK_SPACE:
                Mix_PlayChannel(-1, sons.saut, 0); // Jouer le son de saut
                touches->saut = 1;
                break;
            case SDLK_ESCAPE:
                *continuer = 0;
                break;
            }
            break;

        case SDL_KEYUP:
            switch (event.key.keysym.sym)
            {
            case SDLK_q:
                touches->gauche = 0;
                break;
            case SDLK_d:
                touches->droite = 0;
                break;
            }
            break;
        }
    }
}

int detecterCollision(SDL_Rect joueur)
{
    // Determiner les cotés du joueur
    int gauche = joueur.x / BLOC_SIZE;
    int droite = (joueur.x + joueur.w - 1) / BLOC_SIZE;
    int haut = joueur.y / BLOC_SIZE;
    int bas = (joueur.y + joueur.h - 1) / BLOC_SIZE;

    // Vérifier les limites de la carte
    if (gauche < 0)
        gauche = 0;
    if (droite >= MAP_LARGEUR)
        droite = MAP_LARGEUR - 1;
    if (haut < 0)
        haut = 0;
    if (bas >= MAP_HAUTEUR)
        bas = MAP_HAUTEUR - 1;

    // Vérifier les collisions avec les blocs
    for (int y = haut; y <= bas; y++)
    {
        for (int x = gauche; x <= droite; x++)
        {
            if (x < 0 || x >= MAP_LARGEUR || y < 0 || y >= MAP_HAUTEUR)
                continue;

            int bloc = map[y][x];

            // On ignore les pièces et TOAD
            if (bloc != 0 && bloc != PIECE && bloc != TOAD)
            {
                // Si c'est un bloc mystère, vérifier si Mario le touche par dessous
                if (bloc == BLOC_RECOMPENSE)
                {
                    int hautBloc = y * BLOC_SIZE;
                    int basJoueur = joueur.y + joueur.h;

                    // Collision par en dessous > on ne bloque pas
                    if (basJoueur <= hautBloc + 5)
                        continue;
                }

                return 1; // Collision détectée
            }
        }
    }

    return 0; // Pas de collision
}