#include "../fonction.h"

//---------------------------------------------------------
// Collisions / Interaction basique avec les ennemis

int detecterCollisionEnnemi(SDL_Rect joueur)
{
    for (int i = 0; i < MAX_ENNEMIS; i++)
    {
        if (ennemis[i].actif)
        {
            if (joueur.x < ennemis[i].rect.x + ennemis[i].rect.w &&
                joueur.x + joueur.w > ennemis[i].rect.x &&
                joueur.y < ennemis[i].rect.y + ennemis[i].rect.h &&
                joueur.y + joueur.h > ennemis[i].rect.y)
            {
                int basJoueur = joueur.y + joueur.h;
                int hautEnnemi = ennemis[i].rect.y;

                if (basJoueur >= hautEnnemi - 5 && basJoueur <= hautEnnemi + 10)
                {
                    continue;
                }

                return 1;
            }
        }
    }
    return 0;
}

int sauterSurEnnemi(SDL_Rect joueur, float vitesseSaut, ScoreJeu *scoreData)
{
    if (vitesseSaut > 0)
    {
        for (int i = 0; i < MAX_ENNEMIS; i++)
        {
            if (ennemis[i].actif)
            {
                int basJoueur = joueur.y + joueur.h;
                int hautEnnemi = ennemis[i].rect.y;

                if (basJoueur >= hautEnnemi - 5 && basJoueur <= hautEnnemi + 10)
                {
                    if (joueur.x < ennemis[i].rect.x + ennemis[i].rect.w &&
                        joueur.x + joueur.w > ennemis[i].rect.x)
                    {
                        ajouterEffetEcrasement(ennemis[i].rect.x, ennemis[i].rect.y);

                        if (ennemis[i].type == KOOPA)
                        {
                            for (int j = 0; j < MAX_ENNEMIS; j++)
                            {
                                if (!carapaces[j].actif)
                                {
                                    carapaces[j].rect = ennemis[i].rect;
                                    carapaces[j].vitesse = 0;
                                    carapaces[j].actif = 1;
                                    break;
                                }
                            }
                        }

                        // Il gagne 400 points
                        scoreData->score += 400;
                        ennemis[i].actif = 0;
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

//---------------------------------------------------------
// Gestion des affichages apres collision avec les ennemis

Effet effets[MAX_ENNEMIS];

void initialiserEffets()
{
    for (int i = 0; i < MAX_ENNEMIS; i++)
    {
        effets[i].actif = 0;
    }
}

void ajouterEffetEcrasement(int x, int y)
{
    for (int i = 0; i < MAX_ENNEMIS; i++)
    {
        if (!effets[i].actif)
        {
            effets[i].rect.x = x;
            effets[i].rect.y = y;
            effets[i].rect.w = BLOC_SIZE;
            effets[i].rect.h = BLOC_SIZE;
            effets[i].duree = 30;
            effets[i].actif = 1;
            break;
        }
    }
}

void mettreAJourEffets()
{
    for (int i = 0; i < MAX_ENNEMIS; i++)
    {
        if (effets[i].actif)
        {
            effets[i].duree--;
            if (effets[i].duree <= 0)
            {
                effets[i].actif = 0;
            }
        }
    }
}

void dessinerEffets(SDL_Renderer *renderer, int cameraX, TexturesJeu textures)
{
    for (int i = 0; i < MAX_ENNEMIS; i++)
    {
        if (effets[i].actif)
        {
            SDL_Rect dest = {
                effets[i].rect.x - cameraX,
                effets[i].rect.y,
                effets[i].rect.w,
                effets[i].rect.h};

            SDL_Texture *textureEffet = textures.goombaMort;
            for (int j = 0; j < MAX_ENNEMIS; j++)
            {
                if (ennemis[j].rect.x == effets[i].rect.x && ennemis[j].rect.y == effets[i].rect.y)
                {
                    if (ennemis[j].type == KOOPA)
                    {
                        textureEffet = textures.carapace1;
                    }
                    else if (ennemis[j].type == GOOMBA)
                    {
                        textureEffet = textures.goombaMort;
                    }
                    break;
                }
            }

            SDL_RenderCopy(renderer, textureEffet, NULL, &dest);

            effets[i].duree--;
            if (effets[i].duree <= 0)
            {
                effets[i].actif = 0;
            }
        }
    }
}

//---------------------------------------------------------

Carapace carapaces[MAX_ENNEMIS];

void initialiserCarapaces()
{
    for (int i = 0; i < MAX_ENNEMIS; i++)
    {
        carapaces[i].actif = 0;
        carapaces[i].mobile = 0;
        carapaces[i].vitesse = 0;
        carapaces[i].direction = 0;
        carapaces[i].tempsLancement = 0;
        carapaces[i].rect.x = 0;
        carapaces[i].rect.y = 0;
        carapaces[i].rect.w = BLOC_SIZE;
        carapaces[i].rect.h = BLOC_SIZE;
        carapaces[i].animation = 0;
    }
}

void mettreAJourCarapaces()
{
    static int frames = 0;
    frames++;

    for (int i = 0; i < MAX_ENNEMIS; i++)
    {
        if (!carapaces[i].actif || !carapaces[i].mobile)
            continue;

        // Alterner l'animation toutes les 10 frames
        if (frames % 10 == 0)
            carapaces[i].animation = !carapaces[i].animation;

        carapaces[i].rect.x += carapaces[i].vitesse * carapaces[i].direction;

        if (detecterCollision(carapaces[i].rect))
        {
            carapaces[i].direction *= -1;
            carapaces[i].rect.x += carapaces[i].vitesse * carapaces[i].direction;
        }

        SDL_Rect testSol = carapaces[i].rect;
        testSol.y += 5;
        if (!detecterCollision(testSol))
        {
            carapaces[i].rect.y += 5;
        }

        if (carapaces[i].rect.x < 0 || carapaces[i].rect.x + carapaces[i].rect.w > MAP_LARGEUR * BLOC_SIZE)
        {
            carapaces[i].direction *= -1;
            carapaces[i].rect.x += carapaces[i].vitesse * carapaces[i].direction;
        }
    }
}

int interagirAvecCarapaces(SDL_Rect *joueur, float *vitesseSaut)
{
    int maintenant = (int)SDL_GetTicks(); // Nombre de ms depuis le lancement du programme

    for (int i = 0; i < MAX_ENNEMIS; i++)
    {
        if (!carapaces[i].actif)
            continue;

        SDL_Rect *c = &carapaces[i].rect;

        int basJoueur = joueur->y + joueur->h;
        int hautCarapace = c->y;
        int toucheDessus = (basJoueur >= hautCarapace - 5 && basJoueur <= hautCarapace + 10);

        if (toucheDessus &&
            joueur->x + joueur->w > c->x && joueur->x < c->x + c->w &&
            *vitesseSaut > 0)
        {
            if (carapaces[i].mobile)
            {
                carapaces[i].mobile = 0;
                carapaces[i].vitesse = 0;
            }

            *vitesseSaut = FORCE_SAUT / 1.5f;
            return 0;
        }

        int collisionLaterale =
            joueur->x + joueur->w > c->x && joueur->x < c->x + c->w &&
            joueur->y + joueur->h > c->y && joueur->y < c->y + c->h;

        if (collisionLaterale)
        {
            if (!carapaces[i].mobile)
            {
                // On l’active (Mario l’a tapée en marchant dessus)
                carapaces[i].mobile = 1;
                carapaces[i].vitesse = 6;
                carapaces[i].direction = (joueur->x < c->x) ? DROITE : GAUCHE;
                carapaces[i].tempsLancement = maintenant;
            }
            else
            {
                // Ne tuer Mario que si la carapace est mobile depuis plus de 200 ms
                if (carapaces[i].mobile && maintenant - carapaces[i].tempsLancement >= 200)
                {
                    return 1;
                }
            }
        }
    }

    return 0;
}

void carapacesTuantEnnemis()
{
    int maintenant = (int)SDL_GetTicks();

    for (int i = 0; i < MAX_ENNEMIS; i++)
    {
        if (!carapaces[i].actif || !carapaces[i].mobile)
            continue;
        if (maintenant - carapaces[i].tempsLancement < 300)
            continue;

        SDL_Rect c = carapaces[i].rect;

        for (int j = 0; j < MAX_ENNEMIS; j++)
        {
            if (!ennemis[j].actif)
                continue;

            SDL_Rect e = ennemis[j].rect;

            if (SDL_HasIntersection(&c, &e)) // Vérifier si la carapace touche un ennemi grace à la fonction SDL HasIntersection
            {
                ennemis[j].actif = 0;
            }
        }
    }
}

void dessinerCarapaces(SDL_Renderer *renderer, int cameraX, TexturesJeu textures)
{
    for (int i = 0; i < MAX_ENNEMIS; i++)
    {
        if (carapaces[i].actif)
        {
            SDL_Rect dst = {
                carapaces[i].rect.x - cameraX,
                carapaces[i].rect.y,
                carapaces[i].rect.w,
                carapaces[i].rect.h};
            SDL_Texture *texture = carapaces[i].animation ? textures.carapace1 : textures.carapace2;
            SDL_RenderCopy(renderer, texture, NULL, &dst);
        }
    }
}