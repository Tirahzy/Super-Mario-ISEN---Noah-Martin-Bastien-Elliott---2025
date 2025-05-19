#include "../fonction.h"

//---------------------------------------------------------
// Menu

int gererEvenementsMenu(int *continuer, Bouton boutons[], int nombreBoutons)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            *continuer = 0;
            break;

        case SDL_MOUSEMOTION:
        {
            int mx = event.motion.x, my = event.motion.y;
            for (int i = 0; i < nombreBoutons; i++)
                boutons[i].hover = pointDansRect(mx, my, boutons[i].rect);
        }
        break;

        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                int mx = event.button.x, my = event.button.y;
                for (int i = 0; i < nombreBoutons; i++)
                {
                    if (pointDansRect(mx, my, boutons[i].rect))
                    {
                        return i;
                    }
                }
            }
            break;

        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
                *continuer = 0;
            break;
        }
    }
    return -1;
}