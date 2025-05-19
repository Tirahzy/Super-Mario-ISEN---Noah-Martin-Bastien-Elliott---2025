#include "../fonction.h"

//---------------------------------------------------------
// Boutons pour Menu

void initialiserBoutons(Bouton boutons[], int nombreBoutons, char *labels[])
{
    int largeur = 200;
    int hauteur = 50;
    int espacement = 20;
    int debutY = (LARGEUR_FENETRE - (nombreBoutons * hauteur + (nombreBoutons - 1) * espacement)) / 2;

    for (int i = 0; i < nombreBoutons; i++)
    {
        boutons[i].rect.x = (LONGUEUR_FENETRE - largeur) / 2;
        boutons[i].rect.y = debutY + i * (hauteur + espacement);
        boutons[i].rect.w = largeur;
        boutons[i].rect.h = hauteur;
        boutons[i].texte = labels[i]; // On pointe sur la chaine constante
        boutons[i].hover = 0;
    }
}

void dessinerBoutons(SDL_Renderer *renderer, Bouton boutons[], int nombreBoutons, TTF_Font *police)
{
    for (int i = 0; i < nombreBoutons; i++)
    {
        // Couleur du bouton selon son état
        if (boutons[i].hover == -1)
        {
            SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255); // Gris foncé : désactivé
        }
        else if (boutons[i].hover == 1)
        {
            SDL_SetRenderDrawColor(renderer, 100, 150, 255, 255); // Bleu clair : survolé
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, 70, 70, 200, 255); // Bleu normal
        }

        SDL_RenderFillRect(renderer, &boutons[i].rect);

        // Texte en blanc sauf si désactivé
        SDL_Color couleurTexte = (boutons[i].hover == -1) ? (SDL_Color){180, 180, 180} : (SDL_Color){255, 255, 255};

        SDL_Surface *surfaceTexte = TTF_RenderText_Solid(police, boutons[i].texte, couleurTexte);
        if (!surfaceTexte)
        {
            printf("Erreur TTF_RenderText_Solid\n");
            continue;
        }

        SDL_Texture *textureTexte = SDL_CreateTextureFromSurface(renderer, surfaceTexte);
        if (!textureTexte)
        {
            printf("Erreur SDL_CreateTextureFromSurface\n");
            SDL_FreeSurface(surfaceTexte);
            continue;
        }

        SDL_Rect destTexte = {
            boutons[i].rect.x + (boutons[i].rect.w - surfaceTexte->w) / 2,
            boutons[i].rect.y + (boutons[i].rect.h - surfaceTexte->h) / 2,
            surfaceTexte->w,
            surfaceTexte->h};

        SDL_RenderCopy(renderer, textureTexte, NULL, &destTexte);

        SDL_FreeSurface(surfaceTexte);
        SDL_DestroyTexture(textureTexte);
    }
}

int pointDansRect(int x, int y, SDL_Rect rect)
{
    return (x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h);
}