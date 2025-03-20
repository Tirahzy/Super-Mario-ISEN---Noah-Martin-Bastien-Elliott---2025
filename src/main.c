#include "fonction.h"

int main()
{
    if (0 != SDL_Init(SDL_INIT_VIDEO))
    {
        printf("Erreur SDL_Init : %s", SDL_GetError());
        return;
    }


    SDL_Window* fenetreJeu = creerFenetre();



    SDL_Quit();
    return 0;
}