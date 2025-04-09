#include "fonction.h"

SDL_Window *creerFenetre(char nom[])
{
    SDL_Window *nv = SDL_CreateWindow(nom, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                      LONGUEUR_FENETRE, LARGEUR_FENETRE, SDL_WINDOW_SHOWN);
    if (nv == NULL)
    {
        printf("Erreur d'allocation !\n");
        SDL_Quit();
        exit(1);
    }
    return nv;
}

SDL_Renderer *creerRenderer(SDL_Window *fenetre)
{
    SDL_Renderer *renderer = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        printf("Erreur d'allocation du renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(fenetre);
        SDL_Quit();
        exit(1);
    }
    return renderer;
}

void dessinerCarre(SDL_Renderer *renderer, SDL_Rect carre)
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &carre);

    SDL_RenderPresent(renderer);
}

void gererEvenements(SDL_bool *continuer, SDL_Rect *carre, SDL_bool *enSaut, float *vitesseSaut, Touches *touches)
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            *continuer = SDL_FALSE;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_q:
                touches->gauche = SDL_TRUE;
                break;

            case SDLK_d:
                touches->droite = SDL_TRUE;
                break;

            case SDLK_SPACE:
                touches->saut = SDL_TRUE;
                break;

            case SDLK_ESCAPE:
                *continuer = SDL_FALSE;
                break;
            }
            break;

        case SDL_KEYUP:
            switch (event.key.keysym.sym)
            {
            case SDLK_q:
                touches->gauche = SDL_FALSE;
                break;

            case SDLK_d:
                touches->droite = SDL_FALSE;
                break;
            }
            break;
        }
    }
}

int map[MAP_HAUTEUR][MAP_LARGEUR];

void initialiserMap()
{
    // Vider la map
    for (int i = 0; i < MAP_HAUTEUR; i++)
        for (int j = 0; j < MAP_LARGEUR; j++)
            map[i][j] = 0;

    // Sol principal (ligne y = 9)
    for (int x = 0; x < MAP_LARGEUR; x++)
        map[MAP_HAUTEUR - 3][x] = 1;

    // Escalier montant (gauche → droite)
    for (int i = 0; i < 5; i++)
        map[MAP_HAUTEUR - 4 - i][5 + i] = 2;

    // Plateforme flottante horizontale
    for (int x = 20; x < 28; x++)
        map[MAP_HAUTEUR - 6][x] = 2;

    // Trou dans le sol (au milieu)
    for (int x = 35; x <= 37; x++)
        map[MAP_HAUTEUR - 3][x] = 0;

    // Mur vertical (collision latérale)
    for (int y = MAP_HAUTEUR - 6; y < MAP_HAUTEUR - 3; y++)
        map[y][45] = 2;

    // Plafond bas (pour test de saut bloqué)
    for (int x = 50; x < 54; x++)
        map[MAP_HAUTEUR - 10][x] = 2;

    // Grotte / Tunnel
    for (int x = 60; x < 68; x++) {
        map[MAP_HAUTEUR - 4][x] = 1; // sol
        map[MAP_HAUTEUR - 5][x] = 0; // vide
        map[MAP_HAUTEUR - 6][x] = 1; // plafond
    }

    // Plateforme très haute (besoin d'élan)
    for (int x = 75; x < 80; x++)
        map[MAP_HAUTEUR - 9][x] = 2;
}


void dessinerMap(SDL_Renderer *renderer, int cameraX)
{
    for (int y = 0; y < MAP_HAUTEUR; y++)
    {
        for (int x = 0; x < MAP_LARGEUR; x++)
        {
            SDL_Rect bloc = {
                x * BLOC_SIZE - cameraX,
                y * BLOC_SIZE,
                BLOC_SIZE,
                BLOC_SIZE};

            switch (map[y][x])
            {
            case 1:                                                 // Sol
                SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255); // marron
                SDL_RenderFillRect(renderer, &bloc);
                break;
            case 2:                                                   // Plateforme
                SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255); // gris
                SDL_RenderFillRect(renderer, &bloc);
                break;
            default:
                // Rien à dessiner
                break;
            }
        }
    }
}

SDL_Texture *chargerTextureBMP(SDL_Renderer *renderer, const char *chemin)
{
    SDL_Surface *surface = SDL_LoadBMP(chemin);
    if (!surface)
    {
        printf("Erreur SDL_LoadBMP : %s\n", SDL_GetError());
        return NULL;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture)
    {
        printf("Erreur SDL_CreateTextureFromSurface : %s\n", SDL_GetError());
        return NULL;
    }

    return texture;
}

SDL_bool detecterCollision(SDL_Rect joueur)
{
    int gauche = joueur.x / BLOC_SIZE;
    int droite = (joueur.x + joueur.w - 1) / BLOC_SIZE;
    int haut = joueur.y / BLOC_SIZE;
    int bas = (joueur.y + joueur.h - 1) / BLOC_SIZE;

    for (int y = haut; y <= bas; y++)
    {
        for (int x = gauche; x <= droite; x++)
        {
            if (x < 0 || x >= MAP_LARGEUR || y < 0 || y >= MAP_HAUTEUR)
                continue;

            if (map[y][x] != 0)
                return SDL_TRUE;
        }
    }
    return SDL_FALSE;
}