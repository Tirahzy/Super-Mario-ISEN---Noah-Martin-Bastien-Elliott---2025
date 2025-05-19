#include "../fonction.h"

//---------------------------------------------------------
// Gestion transition entre les niveaux

int finDeNiveau(SDL_Rect joueur)
{
    int joueurGaucheBloc = (joueur.x / BLOC_SIZE > 0) ? (joueur.x / BLOC_SIZE - 1) : 0;
    int joueurDroiteBloc = ((joueur.x + joueur.w - 1) / BLOC_SIZE + 1 < MAP_LARGEUR) ? ((joueur.x + joueur.w - 1) / BLOC_SIZE + 1) : (MAP_LARGEUR - 1);
    int joueurHautBloc = (joueur.y / BLOC_SIZE > 0) ? (joueur.y / BLOC_SIZE - 1) : 0;
    int joueurBasBloc = ((joueur.y + joueur.h - 1) / BLOC_SIZE + 1 < MAP_HAUTEUR) ? ((joueur.y + joueur.h - 1) / BLOC_SIZE + 1) : (MAP_HAUTEUR - 1);

    for (int y = joueurHautBloc; y <= joueurBasBloc; y++)
    {
        for (int x = joueurGaucheBloc; x <= joueurDroiteBloc; x++)
        {

            if (map[y][x] == TOAD)
            {
                // Calculer les coordonnées réelles du bloc de fin
                SDL_Rect blocFin = {
                    x * BLOC_SIZE,
                    y * BLOC_SIZE,
                    BLOC_SIZE,
                    BLOC_SIZE};

                if (joueur.x < blocFin.x + blocFin.w &&
                    joueur.x + joueur.w > blocFin.x &&
                    joueur.y < blocFin.y + blocFin.h &&
                    joueur.y + joueur.h > blocFin.y)
                {
                    return 1;
                }
            }
        }
    }

    return 0;
}

int gererEvenementsNiveauTermine(int *continuer, Bouton boutons[], int nombreBoutons)
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
            int mouseX = event.motion.x;
            int mouseY = event.motion.y;

            for (int i = 0; i < nombreBoutons; i++)
            {
                if (mouseX >= boutons[i].rect.x && mouseX <= boutons[i].rect.x + boutons[i].rect.w &&
                    mouseY >= boutons[i].rect.y && mouseY <= boutons[i].rect.y + boutons[i].rect.h)
                {
                    boutons[i].hover = 1;
                }
                else
                {
                    boutons[i].hover = 0;
                }
            }
        }
        break;

        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                int mouseX = event.button.x;
                int mouseY = event.button.y;

                for (int i = 0; i < nombreBoutons; i++)
                {
                    if (mouseX >= boutons[i].rect.x && mouseX <= boutons[i].rect.x + boutons[i].rect.w &&
                        mouseY >= boutons[i].rect.y && mouseY <= boutons[i].rect.y + boutons[i].rect.h)
                    {

                        if (i == 0)
                            return ETAT_JEU;
                        else if (i == 1)
                            return ETAT_MENU;
                    }
                }
            }
            break;

        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                return ETAT_MENU;
            }
            break;
        }
    }

    return -1;
}

int gererGameOver(int *continuer, Bouton boutons[], int nombreBoutons)
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
            int mouseX = event.motion.x;
            int mouseY = event.motion.y;

            for (int i = 0; i < nombreBoutons; i++)
            {
                if (mouseX >= boutons[i].rect.x && mouseX <= boutons[i].rect.x + boutons[i].rect.w &&
                    mouseY >= boutons[i].rect.y && mouseY <= boutons[i].rect.y + boutons[i].rect.h)
                {
                    boutons[i].hover = 1;
                }
                else
                {
                    boutons[i].hover = 0;
                }
            }
        }
        break;

        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                int mouseX = event.button.x;
                int mouseY = event.button.y;

                for (int i = 0; i < nombreBoutons; i++)
                {
                    if (mouseX >= boutons[i].rect.x && mouseX <= boutons[i].rect.x + boutons[i].rect.w &&
                        mouseY >= boutons[i].rect.y && mouseY <= boutons[i].rect.y + boutons[i].rect.h)
                    {

                        if (i == 0)
                            return ETAT_JEU;
                        else if (i == 1)
                            return ETAT_MENU;
                    }
                }
            }
            break;

        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                return ETAT_MENU;
            }
            break;
        }
    }

    return -1;
}
