#include "fonction.h"

SDL_Window* creerFenetre(char nom[])
{
    SDL_Window *nv = SDL_CreateWindow(nom , SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        LONGUEUR_FENETRE, LARGEUR_FENETRE, SDL_WINDOW_SHOWN);
    if(nv == NULL)
    {
        printf("Erreur d'allocation !\n");
        SDL_Quit();
        exit(1);
    }
    return nv;
}

SDL_Renderer* creerRenderer(SDL_Window* fenetre)
{
    SDL_Renderer* renderer = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == NULL)
    {
        printf("Erreur d'allocation du renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(fenetre);
        SDL_Quit();
        exit(1);
    }
    return renderer;
}

void dessinerCarre(SDL_Renderer* renderer, SDL_Rect carre)
{
    SDL_SetRenderDrawColor(renderer, 129, 212, 250, 255);
    SDL_RenderClear(renderer);
    
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &carre);

    SDL_RenderPresent(renderer);
}

void gererEvenements(SDL_bool* continuer, SDL_Rect* carre, SDL_bool* enSaut, float* vitesseSaut, Touches* touches)
{
    SDL_Event event;
    
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
                *continuer = SDL_FALSE;
                break;
                
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
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
                switch(event.key.keysym.sym)
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