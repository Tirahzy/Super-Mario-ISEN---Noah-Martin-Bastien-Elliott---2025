#include "../fonction.h"

//---------------------------------------------------------
// Gestion des blocs mystères

int detecterCollisionBlocMystere(SDL_Rect joueur, float vitesseSaut)
{
    if (vitesseSaut >= 0)
        return 0; // On ne vérifie que si Mario saute vers le haut

    int xCentre = (joueur.x + joueur.w / 2) / BLOC_SIZE;
    int yDessus = (joueur.y - 1) / BLOC_SIZE;

    if (xCentre >= 0 && xCentre < MAP_LARGEUR && yDessus >= 0 && yDessus < MAP_HAUTEUR)
    {
        if (map[yDessus][xCentre] == BLOC_RECOMPENSE)
        {
            return 1;
        }
    }

    return 0;
}

Champignon champi = {{0, 0, BLOC_SIZE, BLOC_SIZE}, 0, 0.0f};

void ChampignonSiBlocMystereTouche(SDL_Rect joueur, SDL_Rect *champignon, float vitesseSaut)
{
    int xBloc = (joueur.x + joueur.w / 2) / BLOC_SIZE;
    int yBloc = (joueur.y - 1) / BLOC_SIZE;

    if (map[yBloc][xBloc] == BLOC_RECOMPENSE)
    {
        map[yBloc][xBloc] = BLOC_STRUCTURE;
        champignon->x = xBloc * BLOC_SIZE;
        champignon->y = (yBloc - 1) * BLOC_SIZE;
    }
}