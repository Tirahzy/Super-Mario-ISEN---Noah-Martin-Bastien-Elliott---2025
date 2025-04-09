#include "fonction.h"

int main(int argc, char *argv[])
{
    if (0 != SDL_Init(SDL_INIT_VIDEO))
    {
        printf("Erreur SDL_Init : %s", SDL_GetError());
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

    SDL_Rect carre = {100, SOL, 50, 50};

    SDL_bool enSaut = SDL_FALSE;
    float vitesseSaut = 0;
    Touches touches = {SDL_FALSE, SDL_FALSE, SDL_FALSE};

    int cameraX = 0;
    initialiserMap();

    SDL_bool continuer = SDL_TRUE;
    while (continuer)
    {
        gererEvenements(&continuer, &carre, &enSaut, &vitesseSaut, &touches);

        // if (touches.gauche)
        // {
        //     carre.x -= VITESSE_DEPLACEMENT;
        //     if (carre.x < 0)
        //         carre.x = 0;
        // }
        // if (touches.droite)
        // {
        //     carre.x += VITESSE_DEPLACEMENT;
        //     if (carre.x > MAP_LARGEUR * BLOC_SIZE - carre.w)
        //         carre.x = MAP_LARGEUR * BLOC_SIZE - carre.w;
        // }

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

        // if (enSaut)
        // {
        //     carre.y += vitesseSaut;
        //     vitesseSaut += GRAVITE;

        //     if (carre.y >= SOL)
        //     {
        //         carre.y = SOL;
        //         enSaut = SDL_FALSE;
        //         vitesseSaut = 0;
        //     }
        // }
        // else if (touches.saut)
        // {
        //     enSaut = SDL_TRUE;
        //     vitesseSaut = FORCE_SAUT;
        //     touches.saut = SDL_FALSE;
        // }

        if (enSaut)
        {
            carre.y += vitesseSaut;
            vitesseSaut += GRAVITE;
        
            // Vérifier collision vers le bas
            SDL_Rect testBas = carre;
            testBas.y += 1; // Petit déplacement vers le bas pour détection
            
            if (detecterCollision(testBas))
            {
                // Ajuster la position juste au-dessus du bloc
                carre.y = ((carre.y + carre.h) / BLOC_SIZE) * BLOC_SIZE - carre.h;
                enSaut = SDL_FALSE;
                vitesseSaut = 0;
            }
            
            // Vérifier collision vers le haut
            SDL_Rect testHaut = carre;
            testHaut.y -= 1; // Petit déplacement vers le haut
            
            if (detecterCollision(testHaut) && vitesseSaut < 0)
            {
                carre.y = ((carre.y / BLOC_SIZE) + 1) * BLOC_SIZE;
                vitesseSaut = 0; // Annuler la vitesse ascendante
            }
        }
        else
        {
            // Vérifier si on est sur le sol
            SDL_Rect testSol = carre;
            testSol.y += 1; // Petit déplacement vers le bas
            
            if (!detecterCollision(testSol))
            {
                // On n'est plus sur le sol - commencer à tomber
                enSaut = SDL_TRUE;
                vitesseSaut = 0;
            }
            else if (touches.saut)
            {
                // Saut seulement si on est sur le sol
                enSaut = SDL_TRUE;
                vitesseSaut = FORCE_SAUT;
                touches.saut = SDL_FALSE;
            }
        }

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
        
        // dessinerCarre(renderer, (SDL_Rect){carre.x - cameraX, carre.y, carre.w, carre.h});
        SDL_Rect dst = {carre.x - cameraX, carre.y, carre.w, carre.h};
        SDL_RenderCopy(renderer, persoTexture, NULL, &dst);

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    SDL_DestroyTexture(persoTexture);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(fenetreJeu);
    SDL_Quit();
    return 0;
}