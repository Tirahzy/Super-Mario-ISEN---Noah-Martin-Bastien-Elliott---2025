#include "../fonction.h"

//---------------------------------------------------------
// Fonction de sauvegarde de la partie

int sauvegarderUtilisateur(char nom[], int niveauActuel, int score, int vies)
{
    FILE *f = fopen(FICHIER_SAUVEGARDE, "r");
    Sauvegarde sauvegardes[MAX_SAUVEGARDES];
    int n = 0;
    int trouve = 0;

    if (f)
    {
        while (fscanf(f, "%49s %d %d %d %d %d",
                      sauvegardes[n].nom,
                      &sauvegardes[n].niveauActuel,
                      &sauvegardes[n].niveauMax,
                      &sauvegardes[n].scoreActuel,
                      &sauvegardes[n].scoreMax,
                      &sauvegardes[n].vies) == 6)
        {

            if (strcmp(sauvegardes[n].nom, nom) == 0)
            {
                sauvegardes[n].niveauActuel = niveauActuel;
                if (niveauActuel > sauvegardes[n].niveauMax)
                {
                    sauvegardes[n].niveauMax = niveauActuel;
                }
                if (score > sauvegardes[n].scoreMax)
                {
                    sauvegardes[n].scoreMax = score;
                }
                sauvegardes[n].scoreActuel = score;
                sauvegardes[n].vies = vies;
                trouve = 1;
            }

            n++;
            if (n >= MAX_SAUVEGARDES)
                break;
        }
        fclose(f);
    }

    if (!trouve && n < MAX_SAUVEGARDES)
    {
        strncpy(sauvegardes[n].nom, nom, 49);
        sauvegardes[n].nom[49] = '\0';
        sauvegardes[n].niveauActuel = niveauActuel;
        sauvegardes[n].niveauMax = niveauActuel;
        sauvegardes[n].scoreActuel = score;
        sauvegardes[n].scoreMax = score;
        sauvegardes[n].vies = vies;
        n++;
    }

    f = fopen(FICHIER_SAUVEGARDE, "w");
    if (!f)
    {
        perror("[ERREUR fopen]");
        return 0;
    }

    for (int i = 0; i < n; i++)
    {
        fprintf(f, "%s %d %d %d %d %d\n",
                sauvegardes[i].nom,
                sauvegardes[i].niveauActuel,
                sauvegardes[i].niveauMax,
                sauvegardes[i].scoreActuel,
                sauvegardes[i].scoreMax,
                sauvegardes[i].vies);
    }

    fclose(f);
    return 1;
}

int chargerUtilisateur(char nom[], Sauvegarde *out)
{
    FILE *f = fopen(FICHIER_SAUVEGARDE, "r");
    if (!f)
    {
        printf(">>> Fichier sauvegarde introuvable.\n");
        return 0;
    }

    while (fscanf(f, "%49s %d %d %d %d %d", out->nom, &out->niveauActuel, &out->niveauMax, &out->scoreActuel, &out->scoreMax, &out->vies) == 6)
    {
        if (strcmp(out->nom, nom) == 0)
        {
            fclose(f);
            return 1;
        }
    }
    {
        if (strcmp(out->nom, nom) == 0)
        {
            fclose(f);
            return 1;
        }
    }

    fclose(f);
    return 0;
}

void saisirNomUtilisateur(SDL_Renderer *renderer, TTF_Font *police, char *nom, int maxLen)
{
    SDL_Color couleurTexte = {255, 255, 255};
    SDL_Color couleurFond = {0, 0, 0};

    int nomValide = 0;

    while (!nomValide)
    {
        SDL_StartTextInput();
        SDL_Event event;
        int terminer = 0;
        nom[0] = '\0';

        while (!terminer)
        {
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    terminer = 1;
                    nom[0] = '\0';
                }
                else if (event.type == SDL_TEXTINPUT)
                {
                    if (strlen(nom) + strlen(event.text.text) < maxLen - 1)
                    {
                        strcat(nom, event.text.text); // Pour concatenation
                    }
                }
                else if (event.type == SDL_KEYDOWN)
                {
                    if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(nom) > 0)
                    {
                        nom[strlen(nom) - 1] = '\0';
                    }
                    else if (event.key.keysym.sym == SDLK_RETURN)
                    {
                        terminer = 1;
                    }
                    else if (event.key.keysym.sym == SDLK_ESCAPE)
                    {
                        terminer = 1;
                        nom[0] = '\0'; // Annuler
                    }
                }
            }

            SDL_SetRenderDrawColor(renderer, couleurFond.r, couleurFond.g, couleurFond.b, 255);
            SDL_RenderClear(renderer);

            SDL_Surface *surface = TTF_RenderText_Solid(police, "Entrez votre nom :", couleurTexte);
            SDL_Texture *texteTexture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect dest1 = {100, 100, surface->w, surface->h};
            SDL_RenderCopy(renderer, texteTexture, NULL, &dest1);
            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texteTexture);

            char affichage[128];
            snprintf(affichage, sizeof(affichage), "%s|", nom); // Curseur

            surface = TTF_RenderText_Solid(police, affichage, couleurTexte);
            texteTexture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect dest2 = {100, 160, surface->w, surface->h};
            SDL_RenderCopy(renderer, texteTexture, NULL, &dest2);
            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texteTexture);

            SDL_RenderPresent(renderer);
            SDL_Delay(16);
        }

        SDL_StopTextInput();

        // Vérifie si le nom est non vide et pas que des espaces
        int i, tousEspaces = 1;
        for (i = 0; nom[i] != '\0'; i++)
        {
            if (nom[i] != ' ')
            {
                tousEspaces = 0;
                break;
            }
        }

        if (strlen(nom) > 0 && !tousEspaces)
        {
            nomValide = 1;
        }
        else
        {
            // Message d'erreur simple dans la console
            printf("Nom invalide. Veuillez entrer un nom valide.\n");
            SDL_Delay(500);
        }
    }
}

int afficherChoixChargement(SDL_Renderer *renderer, TTF_Font *police, SDL_Window *fenetre)
{
    Bouton boutons[2] = {
        {{(LONGUEUR_FENETRE - 300) / 2, 200, 300, 60}, "Charger la sauvegarde", 0},
        {{(LONGUEUR_FENETRE - 300) / 2, 300, 300, 60}, "Entrer un autre nom", 0}};

    SDL_Event event;
    int choix = -1;

    while (choix == -1)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                return -1;
            }

            if (event.type == SDL_MOUSEMOTION)
            {
                int mx = event.motion.x, my = event.motion.y;
                for (int i = 0; i < 2; i++)
                    boutons[i].hover = pointDansRect(mx, my, boutons[i].rect);
            }

            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
            {
                int mx = event.button.x, my = event.button.y;
                for (int i = 0; i < 2; i++)
                {
                    if (pointDansRect(mx, my, boutons[i].rect))
                    {
                        choix = i;
                    }
                }
            }

            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    choix = 1;
                else if (event.key.keysym.sym == SDLK_SPACE)
                    choix = 0;
            }
        }

        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        SDL_Color couleurTitre = {255, 255, 255};
        SDL_Surface *titre = TTF_RenderText_Solid(police, "Sauvegarde trouvee :", couleurTitre);
        SDL_Texture *textureTitre = SDL_CreateTextureFromSurface(renderer, titre);
        SDL_Rect rectTitre = {(LONGUEUR_FENETRE - titre->w) / 2, 100, titre->w, titre->h};
        SDL_RenderCopy(renderer, textureTitre, NULL, &rectTitre);
        SDL_FreeSurface(titre);
        SDL_DestroyTexture(textureTitre);

        dessinerBoutons(renderer, boutons, 2, police);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    return choix;
}

int chargerToutesLesSauvegardes(Sauvegarde sauvegardes[], int max)
{
    FILE *f = fopen(FICHIER_SAUVEGARDE, "r");
    if (!f)
        return 0;

    int count = 0;
    while (count < max && fscanf(f, "%49s %d %d %d %d %d",
                                 sauvegardes[count].nom,
                                 &sauvegardes[count].niveauActuel,
                                 &sauvegardes[count].niveauMax,
                                 &sauvegardes[count].scoreActuel,
                                 &sauvegardes[count].scoreMax,
                                 &sauvegardes[count].vies) == 6)
    {
        count++;
    }

    fclose(f);
    return count;
}