#include "fonction.h"

int main(int argc, char *argv[])
{
    if (0 != SDL_Init(SDL_INIT_VIDEO))
    {
        printf("Erreur SDL_Init : %s", SDL_GetError());
        return 1;
    }

    if (TTF_Init() == -1)
    {
        printf("Erreur d'initialisation de SDL_ttf : %s\n", TTF_GetError());
        return 1;
    }

    TTF_Font *police = TTF_OpenFont("fonts/arial.ttf", 24);
    if (!police)
    {
        printf("Erreur chargement police : %s\n", TTF_GetError());
        return 1;
    }

    SDL_Window *fenetreJeu = creerFenetre("Mario");
    SDL_Renderer *renderer = creerRenderer(fenetreJeu);

    SDL_Texture *persoTexture = chargerTextureBMP(renderer, "img/perso.bmp");
    if (!persoTexture)
    {
        printf("Échec du chargement de l'image ! Vérifie le chemin\n");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(fenetreJeu);
        SDL_Quit();
        return 1;
    }

    SDL_Rect carre = {100, (MAP_HAUTEUR - 3) * BLOC_SIZE - 40, TAILLE_PERSO, TAILLE_PERSO};

    SDL_bool enSaut = SDL_FALSE;
    float vitesseSaut = 0;
    Touches touches = {SDL_FALSE, SDL_FALSE, SDL_FALSE};

    int cameraX = 0;
    initialiserMap();

    int nbPieces = 0;

    SDL_bool continuer = SDL_TRUE;
    while (continuer)
    {
        gererEvenements(&continuer, &carre, &enSaut, &vitesseSaut, &touches);

        // Déplacement horizontal avec collision
        SDL_Rect testCarre = carre;
        if (touches.gauche)
        {
            testCarre.x -= VITESSE_DEPLACEMENT;
            if (!detecterCollision(testCarre))
                carre.x = testCarre.x;
        }
        if (touches.droite)
        {
            testCarre.x = carre.x + VITESSE_DEPLACEMENT;
            if (!detecterCollision(testCarre))
                carre.x = testCarre.x;
        }

        // Saut avec collision
        if (enSaut)
        {
            carre.y += vitesseSaut;
            vitesseSaut += GRAVITE;

            // Collision vers le bas
            SDL_Rect testBas = carre;
            testBas.y += 1;
            if (vitesseSaut > 0 && detecterCollision(testBas))
            {
                carre.y = ((carre.y + carre.h) / BLOC_SIZE) * BLOC_SIZE - carre.h;
                enSaut = SDL_FALSE;
                vitesseSaut = 0;
            }

            // Collision vers le haut
            SDL_Rect testHaut = carre;
            testHaut.y -= 1;
            if (vitesseSaut < 0 && detecterCollision(testHaut))
            {
                carre.y = ((carre.y) / BLOC_SIZE + 1) * BLOC_SIZE;
                vitesseSaut = 0;
            }
        }
        else
        {
            // Tester si le joueur est dans les airs
            SDL_Rect testSol = carre;
            testSol.y += 1;
            if (!detecterCollision(testSol))
            {
                enSaut = SDL_TRUE;
                vitesseSaut = 0;
            }
            else if (touches.saut)
            {
                enSaut = SDL_TRUE;
                vitesseSaut = FORCE_SAUT;
                touches.saut = SDL_FALSE;
            }
        }
        // Detection piece
        int gauche = carre.x / BLOC_SIZE;
        int droite = (carre.x + carre.w - 1) / BLOC_SIZE;
        int haut = carre.y / BLOC_SIZE;
        int bas = (carre.y + carre.h - 1) / BLOC_SIZE;

        for (int y = haut; y <= bas; y++)
        {
            for (int x = gauche; x <= droite; x++)
            {
                // grâce aux calculs précédents et aux collisions qui empêchent de sortir
                if (map[y][x] == BLOC_PIECE)
                {
                    map[y][x] = 0; // Supprimer la pièce
                    nbPieces++;
                }
            }
        }

        // Ne pas sortir de la map
        if (carre.x < 0)
            carre.x = 0;
        if (carre.x > MAP_LARGEUR * BLOC_SIZE - carre.w)
            carre.x = MAP_LARGEUR * BLOC_SIZE - carre.w;

        // Mise à jour de la caméra
        cameraX = carre.x + carre.w / 2 - LONGUEUR_FENETRE / 2;
        if (cameraX < 0)
            cameraX = 0;
        if (cameraX > MAP_LARGEUR * BLOC_SIZE - LONGUEUR_FENETRE)
            cameraX = MAP_LARGEUR * BLOC_SIZE - LONGUEUR_FENETRE;

        SDL_SetRenderDrawColor(renderer, 129, 212, 255, 255); // ciel
        SDL_RenderClear(renderer);

        // Décor
        dessinerMap(renderer, cameraX);

        // Perso
        dessinerCarre(renderer, (SDL_Rect){carre.x - cameraX, carre.y, carre.w, carre.h});
        // SDL_Rect dst = {carre.x - cameraX, carre.y, carre.w, carre.h};
        // SDL_RenderCopy(renderer, persoTexture, NULL, &dst);

        // Afficher le score
        afficherScore(renderer, nbPieces, police);

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    SDL_DestroyTexture(persoTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(fenetreJeu);
    TTF_CloseFont(police);
    TTF_Quit();
    SDL_Quit();
    return 0;
}