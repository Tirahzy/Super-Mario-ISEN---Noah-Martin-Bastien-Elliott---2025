#include "fonction.h"

int main(int argc, char *argv[])
{
    if(0 != SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        return 1;
    }

    SDL_Window* fenetreJeu = creerFenetre();
    SDL_Delay(3000);

    SDL_Quit();
    return 0;
}

// mingw32-make
// ./Jeu.exe