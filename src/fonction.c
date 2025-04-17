#include "fonction.h"

int map[MAP_HAUTEUR][MAP_LARGEUR];
Ennemi ennemis[MAX_ENNEMIS];
Effet effets[MAX_EFFETS];

SDL_Window *creerFenetre(char nom[])
{
    SDL_Window *nv = SDL_CreateWindow(nom, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                      LONGUEUR_FENETRE, LARGEUR_FENETRE, SDL_WINDOW_SHOWN);
    if (nv == NULL)
    {
        printf("Erreur d'allocation !\n");
        SDL_Quit();
        exit(1);
    }
    return nv;
}

SDL_Renderer *creerRenderer(SDL_Window *fenetre)
{
    SDL_Renderer *renderer = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        printf("Erreur d'allocation du renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(fenetre);
        SDL_Quit();
        exit(1);
    }
    return renderer;
}

void dessinerCarre(SDL_Renderer *renderer, SDL_Rect carre)
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &carre);
    SDL_RenderPresent(renderer);
}

void gererEvenements(SDL_bool *continuer, SDL_Rect *carre, SDL_bool *enSaut, float *vitesseSaut, Touches *touches)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            *continuer = SDL_FALSE;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
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
            switch (event.key.keysym.sym)
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

void initialiserMap()
{
    // Initialiser la map avec plus d'éléments Mario
    char marioMap[MAP_HAUTEUR][MAP_LARGEUR] = {
        "************************************************************************************************************************************************************************************************",
        "*                                                                                                                                                                                              *",
        "*                        $$$                                $$$                    $$$$$                             $$$                              $$$                                       *",
        "*                       #####                ##########    #####       #           #####                            #####          #                #####                                       *",
        "*                #                                                     ##                                              ##                              ##                                       *",
        "*         $$$   ###    #####       $$$                                ####                      ####                 ####      #                      ####                                      *",
        "*        #####                    #####                                         #              ######               ######                           ######                                     *",
        "*                                                          ####                ###                                                  ###                                                         *",
        "*    ####                                                                    #######                                          ####                                                              *",
        "*                                         ####                                                                       ####                                                                       *",
        "*                                        ######                                           ####                                                     ####                                         *",
        "*                ##  g                                   ######    g         #####                        ####                                     #####                                        *",
        "*              ##########                                                                                                                       ##                g                            *",
        "*                            ##         #####                                             ##########       g                                                  ###########                      *",
        "*     $$                            ###                                                                                     $$$                    $$$                                          *",
        "*    ####      g       g                  g       g         g                 g                   g             g          #####      g            #####                                       *",
        "* P                                                                                                                                                                                           #*",
        "================================================================================================================================================================================================"
    };
    
    // Remplir la map avec des 0 (vide)
    for (int i = 0; i < MAP_HAUTEUR; i++)
        for (int j = 0; j < MAP_LARGEUR; j++)
            map[i][j] = 0;

    // Ajouter les éléments de la map
    for (int y = 0; y < MAP_HAUTEUR; y++)
    {
        for (int x = 0; x < MAP_LARGEUR; x++)
        {
            switch (marioMap[y][x])
            {
            case '=': // Sol
                map[y][x] = 1;
                break;
            case '#': // Blocs
                map[y][x] = 2;
                break;
            case '$': // Pièces
                map[y][x] = BLOC_PIECE;
                break;
            case '?': // Blocs à points d'interrogation (nouveau type)
                map[y][x] = BLOC_QUESTION;
                break;
            case 'T': // Tuyaux (nouveau type)
                map[y][x] = BLOC_TUYAU;
                break;
            }
        }
    }
    
    initialiserEnnemis(marioMap);
}

void initialiserEnnemis(char niveau[][MAP_LARGEUR])
{
    for (int i = 0; i < MAX_ENNEMIS; i++)
    {
        ennemis[i].actif = SDL_FALSE;
    }
    
    int nombreEnnemis = 0;
    
    for (int y = 0; y < MAP_HAUTEUR; y++)
    {
        for (int x = 0; x < MAP_LARGEUR; x++)
        {
            if (niveau[y][x] == 'g' && nombreEnnemis < MAX_ENNEMIS)
            {
                ennemis[nombreEnnemis].rect.x = x * BLOC_SIZE;
                ennemis[nombreEnnemis].rect.y = y * BLOC_SIZE;
                ennemis[nombreEnnemis].rect.w = ENNEMI_LARGEUR;
                ennemis[nombreEnnemis].rect.h = ENNEMI_HAUTEUR;
                ennemis[nombreEnnemis].vitesse = (rand() % 2) ? 2 : -2;
                ennemis[nombreEnnemis].actif = SDL_TRUE;
                nombreEnnemis++;
            }
        }
    }
}

void initialiserEffets()
{
    for (int i = 0; i < MAX_EFFETS; i++)
    {
        effets[i].actif = SDL_FALSE;
    }
}

void ajouterEffetEcrasement(int x, int y)
{
    for (int i = 0; i < MAX_EFFETS; i++)
    {
        if (!effets[i].actif)
        {
            effets[i].rect.x = x;
            effets[i].rect.y = y;
            effets[i].rect.w = ENNEMI_LARGEUR;
            effets[i].rect.h = ENNEMI_HAUTEUR / 2;  // Écrasé, donc moitié hauteur
            effets[i].duree = 15;  // Durée en frames
            effets[i].actif = SDL_TRUE;
            break;
        }
    }
}

void mettreAJourEffets()
{
    for (int i = 0; i < MAX_EFFETS; i++)
    {
        if (effets[i].actif)
        {
            effets[i].duree--;
            if (effets[i].duree <= 0)
            {
                effets[i].actif = SDL_FALSE;
            }
        }
    }
}

void dessinerEffets(SDL_Renderer *renderer, int cameraX)
{
    for (int i = 0; i < MAX_EFFETS; i++)
    {
        if (effets[i].actif)
        {
            SDL_Rect effetEcran = {
                effets[i].rect.x - cameraX,
                effets[i].rect.y + ENNEMI_HAUTEUR / 2,  // Positionner en bas de l'ennemi original
                effets[i].rect.w,
                effets[i].rect.h
            };
            
            SDL_SetRenderDrawColor(renderer, 150, 0, 0, 255);  // Rouge plus foncé
            SDL_RenderFillRect(renderer, &effetEcran);
        }
    }
}

void dessinerMap(SDL_Renderer *renderer, int cameraX)
{
    for (int y = 0; y < MAP_HAUTEUR; y++)
    {
        for (int x = 0; x < MAP_LARGEUR; x++)
        {
            SDL_Rect bloc = {
                x * BLOC_SIZE - cameraX,
                y * BLOC_SIZE,
                BLOC_SIZE,
                BLOC_SIZE};

            switch (map[y][x])
            {
            case 1: // Sol - couleur marron
                SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
                SDL_RenderFillRect(renderer, &bloc);
                break;
            case 2: // Blocs - couleur bleue/grise
                SDL_SetRenderDrawColor(renderer, 80, 80, 200, 255);
                SDL_RenderFillRect(renderer, &bloc);
                break;
            case BLOC_PIECE: // Pièces - couleur or
                SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
                SDL_RenderFillRect(renderer, &bloc);
                break;
            case BLOC_QUESTION: // Blocs à points d'interrogation - orange
                SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255);
                SDL_RenderFillRect(renderer, &bloc);
                break;
            case BLOC_TUYAU: // Tuyaux - vert
                SDL_SetRenderDrawColor(renderer, 0, 150, 0, 255);
                SDL_RenderFillRect(renderer, &bloc);
                break;
            default:
                break;
            }
        }
    }
}

void dessinerEnnemis(SDL_Renderer *renderer, int cameraX)
{
    for (int i = 0; i < MAX_ENNEMIS; i++)
    {
        if (ennemis[i].actif)
        {
            SDL_Rect ennemiEcran = {
                ennemis[i].rect.x - cameraX,
                ennemis[i].rect.y,
                ennemis[i].rect.w,
                ennemis[i].rect.h
            };
            
            if (ennemiEcran.x + ennemiEcran.w >= 0 && ennemiEcran.x <= LONGUEUR_FENETRE)
            {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderFillRect(renderer, &ennemiEcran);
            }
        }
    }
}

SDL_bool detecterCollisionEntreEnnemis(SDL_Rect ennemi, int indexEnnemi)
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
                return SDL_TRUE;
            }
        }
    }
    return SDL_FALSE;
}

void mettreAJourEnnemis()
{
    for (int i = 0; i < MAX_ENNEMIS; i++)
    {
        if (ennemis[i].actif)
        {
            // Déplacement horizontal
            SDL_Rect testRect = ennemis[i].rect;
            testRect.x += ennemis[i].vitesse;
            
            // Vérifier collision avec les murs
            if (detecterCollision(testRect))
            {
                ennemis[i].vitesse = -ennemis[i].vitesse;
            }
            // Vérifier collision avec d'autres ennemis
            else if (detecterCollisionEntreEnnemis(testRect, i))
            {
                ennemis[i].vitesse = -ennemis[i].vitesse;
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
                    ennemis[i].vitesse = -ennemis[i].vitesse;
                }
                else
                {
                    // Déplacer l'ennemi
                    ennemis[i].rect.x += ennemis[i].vitesse;
                }
            }
            
            // Appliquer la gravité aux ennemis s'ils ne sont pas sur le sol
            SDL_Rect testChute = ennemis[i].rect;
            testChute.y += 1;
            
            if (!detecterCollision(testChute))
            {
                // Appliquer la gravité
                ennemis[i].rect.y += 5;
                
                // Vérifier si l'ennemi est tombé au sol
                testChute = ennemis[i].rect;
                testChute.y += 1;
                
                if (detecterCollision(testChute))
                {
                    // Ajuster la position pour être exactement au niveau du sol
                    ennemis[i].rect.y = ((ennemis[i].rect.y + ennemis[i].rect.h) / BLOC_SIZE) * BLOC_SIZE - ennemis[i].rect.h;
                }
            }
            
            // Vérifier si l'ennemi est sorti de la map
            if (ennemis[i].rect.y > MAP_HAUTEUR * BLOC_SIZE)
            {
                ennemis[i].actif = SDL_FALSE;
            }
        }
    }
}

SDL_bool sauterSurEnnemi(SDL_Rect joueur, float vitesseSaut)
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
                        // Ajouter un effet d'écrasement
                        ajouterEffetEcrasement(ennemis[i].rect.x, ennemis[i].rect.y);
                        
                        // Désactiver l'ennemi
                        ennemis[i].actif = SDL_FALSE;
                        return SDL_TRUE;
                    }
                }
            }
        }
    }
    return SDL_FALSE;
}

SDL_bool detecterCollisionEnnemi(SDL_Rect joueur)
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
                // Vérifier si le joueur tombe sur l'ennemi par le dessus
                int basJoueur = joueur.y + joueur.h;
                int hautEnnemi = ennemis[i].rect.y;
                
                if (basJoueur >= hautEnnemi - 5 && basJoueur <= hautEnnemi + 10)
                {
                    // Si le joueur touche l'ennemi par le dessus, ne pas considérer comme collision
                    continue;
                }
                
                return SDL_TRUE;
            }
        }
    }
    return SDL_FALSE;
}

SDL_Texture *chargerTextureBMP(SDL_Renderer *renderer, const char *chemin)
{
    SDL_Surface *surface = SDL_LoadBMP(chemin);
    if (!surface)
    {
        printf("Erreur SDL_LoadBMP : %s\n", SDL_GetError());
        return NULL;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture)
    {
        printf("Erreur SDL_CreateTextureFromSurface : %s\n", SDL_GetError());
        return NULL;
    }

    return texture;
}

SDL_bool detecterCollision(SDL_Rect joueur)
{
    int gauche = joueur.x / BLOC_SIZE;
    int droite = (joueur.x + joueur.w - 1) / BLOC_SIZE;
    int haut = joueur.y / BLOC_SIZE;
    int bas = (joueur.y + joueur.h - 1) / BLOC_SIZE;

    for (int y = haut; y <= bas; y++)
    {
        for (int x = gauche; x <= droite; x++)
        {
            if (x < 0 || x >= MAP_LARGEUR || y < 0 || y >= MAP_HAUTEUR)
                continue;

            if (map[y][x] != 0 && map[y][x] != BLOC_PIECE)
                return SDL_TRUE;
        }
    }
    return SDL_FALSE;
}

void afficherScore(SDL_Renderer *renderer, int nbPieces, TTF_Font *police)
{
    char texte[32];
    sprintf(texte, "Score : %d", nbPieces);

    SDL_Color couleur = {255, 255, 255};
    SDL_Surface *surfaceTexte = TTF_RenderText_Solid(police, texte, couleur);
    SDL_Texture *textureTexte = SDL_CreateTextureFromSurface(renderer, surfaceTexte);

    SDL_Rect dest = {10, 10, surfaceTexte->w, surfaceTexte->h};
    SDL_RenderCopy(renderer, textureTexte, NULL, &dest);

    SDL_FreeSurface(surfaceTexte);
    SDL_DestroyTexture(textureTexte);
}

void initialiserBoutons(Bouton boutons[], int nombreBoutons)
{
    int largeur = 200;
    int hauteur = 50;
    int espacement = 20;
    int debutY = (LARGEUR_FENETRE - (nombreBoutons * hauteur + (nombreBoutons - 1) * espacement)) / 2;
    
    char* textes[] = {"Jouer", "Quitter"};
    
    for (int i = 0; i < nombreBoutons; i++)
    {
        boutons[i].rect.x = (LONGUEUR_FENETRE - largeur) / 2;
        boutons[i].rect.y = debutY + i * (hauteur + espacement);
        boutons[i].rect.w = largeur;
        boutons[i].rect.h = hauteur;
        boutons[i].texte = textes[i];
        boutons[i].hover = SDL_FALSE;
    }
}

void dessinerBoutons(SDL_Renderer *renderer, Bouton boutons[], int nombreBoutons, TTF_Font *police)
{
    for (int i = 0; i < nombreBoutons; i++)
    {
        if (boutons[i].hover)
        {
            SDL_SetRenderDrawColor(renderer, 100, 150, 255, 255);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, 70, 70, 200, 255);
        }
        
        SDL_RenderFillRect(renderer, &boutons[i].rect);
        
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &boutons[i].rect);
        
        SDL_Color couleurTexte = {255, 255, 255};
        SDL_Surface *surfaceTexte = TTF_RenderText_Solid(police, boutons[i].texte, couleurTexte);
        SDL_Texture *textureTexte = SDL_CreateTextureFromSurface(renderer, surfaceTexte);
        
        SDL_Rect destTexte = {
            boutons[i].rect.x + (boutons[i].rect.w - surfaceTexte->w) / 2,
            boutons[i].rect.y + (boutons[i].rect.h - surfaceTexte->h) / 2,
            surfaceTexte->w,
            surfaceTexte->h
        };
        
        SDL_RenderCopy(renderer, textureTexte, NULL, &destTexte);
        
        SDL_FreeSurface(surfaceTexte);
        SDL_DestroyTexture(textureTexte);
    }
}

int gererEvenementsMenu(SDL_bool *continuer, Bouton boutons[], int nombreBoutons)
{
    SDL_Event event;
    int choix = -1;
    
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            *continuer = SDL_FALSE;
            return ETAT_MENU;
            
        case SDL_MOUSEMOTION:
            {
                int mouseX = event.motion.x;
                int mouseY = event.motion.y;
                
                for (int i = 0; i < nombreBoutons; i++)
                {
                    if (mouseX >= boutons[i].rect.x && mouseX <= boutons[i].rect.x + boutons[i].rect.w &&
                        mouseY >= boutons[i].rect.y && mouseY <= boutons[i].rect.y + boutons[i].rect.h)
                    {
                        boutons[i].hover = SDL_TRUE;
                    }
                    else
                    {
                        boutons[i].hover = SDL_FALSE;
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
                        {
                            return ETAT_JEU;
                        }
                        else if (i == 1)
                        {
                            *continuer = SDL_FALSE;
                            return ETAT_MENU;
                        }
                    }
                }
            }
            break;
            
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                *continuer = SDL_FALSE;
                return ETAT_MENU;
            }
            break;
        }
    }
    
    return choix;
}