#include "fonction.h"

const int LONGUEUR_FENETRE = 960; //960
const int LARGEUR_FENETRE = 540; // 540
const int SOL = 380;
const float GRAVITE = 0.5f;
const float FORCE_SAUT = -10.0f;
const int VITESSE_DEPLACEMENT = 5;

int main(int argc, char *argv[])
{
    if(0 != SDL_Init(SDL_INIT_VIDEO))
    {
        printf("Erreur SDL_Init : %s", SDL_GetError());
        return 1;
    }

    SDL_Window* fenetreJeu = creerFenetre("Mario");
    SDL_Renderer* renderer = creerRenderer(fenetreJeu);

    SDL_Rect carre = {50, SOL, 50, 50};
    SDL_bool enSaut = SDL_FALSE;
    float vitesseSaut = 0;
    Touches touches = {SDL_FALSE, SDL_FALSE, SDL_FALSE};

    SDL_bool continuer = SDL_TRUE;
    while(continuer)
    {
        gererEvenements(&continuer, &carre, &enSaut, &vitesseSaut, &touches);
        
        if (touches.gauche)
        {
            carre.x -= VITESSE_DEPLACEMENT;
            if(carre.x < 0) carre.x = 0;
        }
        if (touches.droite)
        {
            carre.x += VITESSE_DEPLACEMENT;
            if(carre.x > LONGUEUR_FENETRE - carre.w) carre.x = LONGUEUR_FENETRE - carre.w;
        }
        
        if (enSaut)
        {
            carre.y += vitesseSaut;
            vitesseSaut += GRAVITE;
            
            if (carre.y >= SOL)
            {
                carre.y = SOL;
                enSaut = SDL_FALSE;
                vitesseSaut = 0;
            }
        }
        else if (touches.saut)
        {
            enSaut = SDL_TRUE;
            vitesseSaut = FORCE_SAUT;
            touches.saut = SDL_FALSE;
        }
        
        dessinerCarre(renderer, carre);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(fenetreJeu);
    SDL_Quit();
    return 0;
}