#include "../fonction.h"

//---------------------------------------------------------
// Deplacement ennemis

void mettreAJourEnnemis()
{
    static int frames = 0;
    frames++;

    for (int i = 0; i < MAX_ENNEMIS; i++)
    {
        if (ennemis[i].actif)
        {
            // Animation qui change toutes les 15 frames (pour donner l'impression de mouvement)
            if (frames % 15 == 0)
            {
                ennemis[i].animation = !ennemis[i].animation;
            }

            // Déplacement horizontal
            SDL_Rect testRect = ennemis[i].rect;
            testRect.x += ennemis[i].direction;

            // Vérifier collision avec les murs
            if (detecterCollision(testRect))
            {
                ennemis[i].direction = -ennemis[i].direction;
            }

            // Vérifier collision avec d'autres ennemis
            else if (detecterCollisionEntreEnnemis(testRect, i))
            {
                ennemis[i].direction = -ennemis[i].direction;
            }
            else
            {
                // Vérifier si l'ennemi a du sol sous lui
                SDL_Rect testSol = testRect;
                testSol.y += 5;
                testSol.w -= 10;
                testSol.x += 5;

                if (!detecterCollision(testSol))
                {
                    // Pas de sol, changer de direction
                    ennemis[i].direction = -ennemis[i].direction;
                }
                else
                {
                    ennemis[i].rect.x += ennemis[i].direction;
                }
            }

            // Appliquer la gravité aux ennemis s'ils ne sont pas sur le sol
            SDL_Rect testChute = ennemis[i].rect;
            testChute.y += 1;

            if (!detecterCollision(testChute))
            {
                // Pour appliquer la gravité
                ennemis[i].rect.y += 5;

                testChute = ennemis[i].rect;
                testChute.y += 1;

                if (detecterCollision(testChute))
                {

                    ennemis[i].rect.y = ((ennemis[i].rect.y + ennemis[i].rect.h) / BLOC_SIZE) * BLOC_SIZE - ennemis[i].rect.h;
                }
            }

            if (ennemis[i].rect.y > MAP_HAUTEUR * BLOC_SIZE)
            {
                ennemis[i].actif = 0;
            }
        }
    }
}

int detecterCollisionEntreEnnemis(SDL_Rect ennemi, int indexEnnemi)
{
    for (int i = 0; i < MAX_ENNEMIS; i++)
    {
        if (i != indexEnnemi && ennemis[i].actif)
        {
            if (ennemi.x < ennemis[i].rect.x + ennemis[i].rect.w &&
                ennemi.x + ennemi.w > ennemis[i].rect.x &&
                ennemi.y < ennemis[i].rect.y + ennemis[i].rect.h &&
                ennemi.y + ennemi.h > ennemis[i].rect.y)
            {
                return 1;
            }
        }
    }
    return 0;
}