#include "fonction.h"

const int LONGUEUR_FENETRE = 960;
const int LARGEUR_FENETRE = 540;

int main(int argc, char *argv[])
{
    if(0 != SDL_Init(SDL_INIT_VIDEO))
    {
        printf("Erreur SDL_Init : %s", SDL_GetError());
        return 1;
    }

    SDL_Window* fenetreJeu = creerFenetre("Mario");
    SDL_Renderer* renderer = creerRenderer(fenetreJeu);

    SDL_Rect carre = {50, 380, 50, 50};

    SDL_bool continuer = SDL_TRUE;
    while(continuer)
    {
        gererEvenements(&continuer, &carre);
        dessinerCarre(renderer, carre);
        SDL_Delay(16); // Petite pause pour économiser le CPU (>60 FPS)
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(fenetreJeu);
    SDL_Quit();
    return 0;
}

// mingw32-make
// ./Jeu.exe