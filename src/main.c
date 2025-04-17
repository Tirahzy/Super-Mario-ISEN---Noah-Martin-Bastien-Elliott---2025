#include "fonction.h"

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Erreur SDL_Init : %s", SDL_GetError());
        return 1;
    }

    if (TTF_Init() == -1)
    {
        printf("Erreur d'initialisation de SDL_ttf : %s\n", TTF_GetError());
        return 1;
    }

    TTF_Font *police = TTF_OpenFont("fonts/arial.ttf", 24);
    if (!police)
    {
        printf("Erreur chargement police : %s\n", TTF_GetError());
        return 1;
    }

    SDL_Window *fenetreJeu = creerFenetre("Mario");
    SDL_Renderer *renderer = creerRenderer(fenetreJeu);

    SDL_Texture *persoTexture = chargerTextureBMP(renderer, "img/perso.bmp");
    if (!persoTexture)
    {
        printf("Échec du chargement de l'image ! Vérifie le chemin\n");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(fenetreJeu);
        TTF_CloseFont(police);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Rect carre = {100, (MAP_HAUTEUR - 3) * BLOC_SIZE - 40, TAILLE_PERSO, TAILLE_PERSO};
    SDL_bool enSaut = SDL_FALSE;
    float vitesseSaut = 0;
    Touches touches = {SDL_FALSE, SDL_FALSE, SDL_FALSE};
    int cameraX = 0;
    int nbPieces = 0;
    int scoreGameOver = 0;

    char niveau1[MAP_HAUTEUR][MAP_LARGEUR] = {
        "************************************************************************************************************************************************************************************************",
        "*                                                                                                                                                                                              *",
        "*                                                      $$$                   $$$$$                                $                                                                            *",
        "*                    #####                            #####                                                      ###                              $$$                                          *",
        "*                                                                              ##                                                               #####                                          *",
        "*         $$        ######      $$$                                           ####                                                      ####                                                   *",
        "*        ####                  #####                                                          $$$$                                   ####     #                                                *",
        "*                                                                                                                                 ####                                                         *",
        "*    ###                                                                     ####     g                                      ####                                                              *",
        "*                                                                                                                     ####                                                                     *",
        "*                                                                                                               ####                            ###                                            *",
        "*                                                                         ######    g         #####                                        #####      g                                        *",
        "*              ##########                                                                                                               ##                                                     *",
        "*                            ##                                                                     ##########       g                                  ###########                            *",
        "*                                    ###                                                                                                                                                       *",
        "*                    g                                    g                    g                             g             g                      g                                            *",
        "* P                                                                                                                                                                                           #*",
        "================================================================================================================================================================================================"};

    initialiserMap();
    initialiserEnnemis(niveau1);

    Bouton boutonsMenu[2];
    initialiserBoutons(boutonsMenu, 2);

    Bouton boutonsGameOver[2];
    int largeur = 200;
    int hauteur = 50;
    int espacement = 20;
    int debutY = (LARGEUR_FENETRE - (2 * hauteur + espacement)) / 2 + 50;

    boutonsGameOver[0].rect.x = (LONGUEUR_FENETRE - largeur) / 2;
    boutonsGameOver[0].rect.y = debutY;
    boutonsGameOver[0].rect.w = largeur;
    boutonsGameOver[0].rect.h = hauteur;
    boutonsGameOver[0].texte = "Rejouer";
    boutonsGameOver[0].hover = SDL_FALSE;

    boutonsGameOver[1].rect.x = (LONGUEUR_FENETRE - largeur) / 2;
    boutonsGameOver[1].rect.y = debutY + hauteur + espacement;
    boutonsGameOver[1].rect.w = largeur;
    boutonsGameOver[1].rect.h = hauteur;
    boutonsGameOver[1].texte = "Menu Principal";
    boutonsGameOver[1].hover = SDL_FALSE;

    int etatJeu = ETAT_MENU;
    SDL_bool continuer = SDL_TRUE;

    while (continuer)
    {
        switch (etatJeu)
        {
        case ETAT_MENU:
            etatJeu = gererEvenementsMenu(&continuer, boutonsMenu, 2);
            if (etatJeu == -1)
                etatJeu = ETAT_MENU;

            if (etatJeu == ETAT_JEU)
            {
                carre.x = 100;
                carre.y = (MAP_HAUTEUR - 3) * BLOC_SIZE - 40;
                nbPieces = 0;
                initialiserMap();
                initialiserEnnemis(niveau1);
            }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            SDL_Color couleurTitre = {255, 255, 255};
            SDL_Surface *surfaceTitre = TTF_RenderText_Solid(police, "Super Mario ISEN", couleurTitre);
            SDL_Texture *textureTitre = SDL_CreateTextureFromSurface(renderer, surfaceTitre);

            SDL_Rect destTitre = {
                (LONGUEUR_FENETRE - surfaceTitre->w) / 2,
                100,
                surfaceTitre->w,
                surfaceTitre->h};

            SDL_RenderCopy(renderer, textureTitre, NULL, &destTitre);
            SDL_FreeSurface(surfaceTitre);
            SDL_DestroyTexture(textureTitre);

            dessinerBoutons(renderer, boutonsMenu, 2, police);
            SDL_RenderPresent(renderer);
            break;

        case ETAT_JEU:
            gererEvenements(&continuer, &carre, &enSaut, &vitesseSaut, &touches);

            SDL_Rect testCarre = carre;
            if (touches.gauche)
            {
                testCarre.x -= VITESSE_DEPLACEMENT;
                if (!detecterCollision(testCarre))
                    carre.x = testCarre.x;
            }
            if (touches.droite)
            {
                testCarre.x = carre.x + VITESSE_DEPLACEMENT;
                if (!detecterCollision(testCarre))
                    carre.x = testCarre.x;
            }

            if (enSaut)
            {
                carre.y += vitesseSaut;
                vitesseSaut += GRAVITE;

                SDL_Rect testBas = carre;
                testBas.y += 1;
                if (vitesseSaut > 0 && detecterCollision(testBas))
                {
                    carre.y = ((carre.y + carre.h) / BLOC_SIZE) * BLOC_SIZE - carre.h;
                    enSaut = SDL_FALSE;
                    vitesseSaut = 0;
                }

                SDL_Rect testHaut = carre;
                testHaut.y -= 1;
                if (vitesseSaut < 0 && detecterCollision(testHaut))
                {
                    carre.y = ((carre.y) / BLOC_SIZE + 1) * BLOC_SIZE;
                    vitesseSaut = 0;
                }
            }
            else
            {
                SDL_Rect testSol = carre;
                testSol.y += 1;
                if (!detecterCollision(testSol))
                {
                    enSaut = SDL_TRUE;
                    vitesseSaut = 0;
                }
                else if (touches.saut)
                {
                    enSaut = SDL_TRUE;
                    vitesseSaut = FORCE_SAUT;
                    touches.saut = SDL_FALSE;
                }
            }
            
            if (enSaut)
            {
                if (sauterSurEnnemi(carre, vitesseSaut))
                {
                    
                    vitesseSaut = FORCE_SAUT / 1.5f;
                }
            }

            mettreAJourEnnemis();
            mettreAJourEffets();

            if (detecterCollisionEnnemi(carre))
            {
                scoreGameOver = nbPieces;
                etatJeu = ETAT_GAME_OVER;
            }

            int gauche = carre.x / BLOC_SIZE;
            int droite = (carre.x + carre.w - 1) / BLOC_SIZE;
            int haut = carre.y / BLOC_SIZE;
            int bas = (carre.y + carre.h - 1) / BLOC_SIZE;

            for (int y = haut; y <= bas; y++)
            {
                for (int x = gauche; x <= droite; x++)
                {
                    if (x >= 0 && x < MAP_LARGEUR && y >= 0 && y < MAP_HAUTEUR && map[y][x] == BLOC_PIECE)
                    {
                        map[y][x] = 0;
                        nbPieces++;
                    }
                }
            }

            if (carre.x < 0)
                carre.x = 0;
            if (carre.x > MAP_LARGEUR * BLOC_SIZE - carre.w)
                carre.x = MAP_LARGEUR * BLOC_SIZE - carre.w;

            cameraX = carre.x + carre.w / 2 - LONGUEUR_FENETRE / 2;
            if (cameraX < 0)
                cameraX = 0;
            if (cameraX > MAP_LARGEUR * BLOC_SIZE - LONGUEUR_FENETRE)
                cameraX = MAP_LARGEUR * BLOC_SIZE - LONGUEUR_FENETRE;

            SDL_SetRenderDrawColor(renderer, 129, 212, 255, 255);
            SDL_RenderClear(renderer);

            dessinerMap(renderer, cameraX);
            dessinerEnnemis(renderer, cameraX);
            dessinerEffets(renderer, cameraX); // Ajouter cette ligne

            SDL_Rect dst = {carre.x - cameraX, carre.y, carre.w, carre.h};
            SDL_RenderCopy(renderer, persoTexture, NULL, &dst);

            afficherScore(renderer, nbPieces, police);
            SDL_RenderPresent(renderer);
            break;

        case ETAT_GAME_OVER:
        {
            SDL_Event event;
            int choix = -1;

            while (SDL_PollEvent(&event))
            {
                switch (event.type)
                {
                case SDL_QUIT:
                    continuer = SDL_FALSE;
                    break;

                case SDL_MOUSEMOTION:
                {
                    int mouseX = event.motion.x;
                    int mouseY = event.motion.y;

                    for (int i = 0; i < 2; i++)
                    {
                        if (mouseX >= boutonsGameOver[i].rect.x && mouseX <= boutonsGameOver[i].rect.x + boutonsGameOver[i].rect.w &&
                            mouseY >= boutonsGameOver[i].rect.y && mouseY <= boutonsGameOver[i].rect.y + boutonsGameOver[i].rect.h)
                        {
                            boutonsGameOver[i].hover = SDL_TRUE;
                        }
                        else
                        {
                            boutonsGameOver[i].hover = SDL_FALSE;
                        }
                    }
                }
                break;

                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT)
                    {
                        int mouseX = event.button.x;
                        int mouseY = event.button.y;

                        for (int i = 0; i < 2; i++)
                        {
                            if (mouseX >= boutonsGameOver[i].rect.x && mouseX <= boutonsGameOver[i].rect.x + boutonsGameOver[i].rect.w &&
                                mouseY >= boutonsGameOver[i].rect.y && mouseY <= boutonsGameOver[i].rect.y + boutonsGameOver[i].rect.h)
                            {
                                choix = i;
                            }
                        }
                    }
                    break;

                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                    {
                        etatJeu = ETAT_MENU;
                    }
                    break;
                }
            }

            if (choix == 0)
            {
                carre.x = 100;
                carre.y = (MAP_HAUTEUR - 3) * BLOC_SIZE - 40;
                nbPieces = 0;
                initialiserMap();
                initialiserEnnemis(niveau1);
                etatJeu = ETAT_JEU;
            }
            else if (choix == 1)
            {
                etatJeu = ETAT_MENU;
            }
        }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            SDL_Color couleurGameOver = {255, 0, 0, 255};
            SDL_Surface *surfaceGameOver = TTF_RenderText_Solid(police, "GAME OVER", couleurGameOver);
            SDL_Texture *textureGameOver = SDL_CreateTextureFromSurface(renderer, surfaceGameOver);

            SDL_Rect destGameOver = {
                (LONGUEUR_FENETRE - surfaceGameOver->w) / 2,
                100,
                surfaceGameOver->w,
                surfaceGameOver->h};

            SDL_RenderCopy(renderer, textureGameOver, NULL, &destGameOver);
            SDL_FreeSurface(surfaceGameOver);
            SDL_DestroyTexture(textureGameOver);

            char textScore[50];
            sprintf(textScore, "Score final : %d", scoreGameOver);

            SDL_Color couleurScore = {255, 255, 255, 255};
            SDL_Surface *surfaceScore = TTF_RenderText_Solid(police, textScore, couleurScore);
            SDL_Texture *textureScore = SDL_CreateTextureFromSurface(renderer, surfaceScore);

            SDL_Rect destScore = {
                (LONGUEUR_FENETRE - surfaceScore->w) / 2,
                170,
                surfaceScore->w,
                surfaceScore->h};

            SDL_RenderCopy(renderer, textureScore, NULL, &destScore);
            SDL_FreeSurface(surfaceScore);
            SDL_DestroyTexture(textureScore);

            dessinerBoutons(renderer, boutonsGameOver, 2, police);
            SDL_RenderPresent(renderer);
            break;
        }

        SDL_Delay(16);
    }

    SDL_DestroyTexture(persoTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(fenetreJeu);
    TTF_CloseFont(police);
    TTF_Quit();
    SDL_Quit();
    return 0;
}