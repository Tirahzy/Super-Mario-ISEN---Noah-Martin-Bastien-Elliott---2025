#include "fonction.h"

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0 || TTF_Init() == -1)
    {
        printf("Erreur SDL ou TTF Init\n");
        return 1;
    }

    TTF_Font *police = TTF_OpenFont("fonts/arial.ttf", 24);
    if (!police)
    {
        printf("Erreur chargement police : %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window *fenetre = creerFenetre("Mario Multiniveau");
    SDL_Renderer *renderer = creerRenderer(fenetre);
    TexturesJeu textures = chargerTextures(renderer);

    if (!textures.perso || !textures.brique || !textures.piece || !textures.tuyau_bas_droite  ||
        !textures.ennemi || !textures.sol || !textures.questionBloc ||
        !textures.tuyau_bas_gauche || !textures.tuyau_haut_droite || !textures.tuyau_haut_gauche)
    {
        fprintf(stderr, "Erreur : une ou plusieurs textures sont nulles\n");
        libererTextures(textures);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(fenetre);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Rect carre = {100, SOL, TAILLE_PERSO, TAILLE_PERSO};
    Touches touches = {SDL_FALSE, SDL_FALSE, SDL_FALSE};
    SDL_bool enSaut = SDL_FALSE, continuer = SDL_TRUE;
    float vitesseSaut = 0;
    int cameraX = 0, nbPieces = 0, scoreFinal = 0, scoreGameOver = 0;
    int current_level = 1;

    GestionnaireFPS fps;
    initialiserFPS(&fps);
    initialiserEffets();
    initialiserMap(current_level);

    Bouton boutonsMenu[2];
    initialiserBoutons(boutonsMenu, 2);

    Bouton boutonsNiveauTermine[2] = {
        {{380, 200, 200, 50}, strdup("Niveau Suivant"), SDL_FALSE},
        {{380, 270, 200, 50}, strdup("Menu Principal"), SDL_FALSE}
    };

    Bouton boutonsGameOver[2] = {
        {{380, 200, 200, 50}, strdup("Rejouer"), SDL_FALSE},
        {{380, 270, 200, 50}, strdup("Menu Principal"), SDL_FALSE}
    };

    int etatJeu = ETAT_MENU;

    while (continuer)
    {
        fps.dernierTemps = SDL_GetTicks();

        switch (etatJeu)
        {
            case ETAT_MENU:
            {
                int choix = gererEvenementsMenu(&continuer, boutonsMenu, 2);
                if (choix == ETAT_JEU)
                {
                    current_level = 1;
                    carre.x = 100;
                    carre.y = SOL;
                    nbPieces = 0;
                    initialiserMap(current_level);
                    etatJeu = ETAT_JEU;
                }

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);
                dessinerBoutons(renderer, boutonsMenu, 2, police);
                SDL_RenderPresent(renderer);
                SDL_Delay(10);
                break;
            }

            case ETAT_JEU:
                gererEvenements(&continuer, &carre, &enSaut, &vitesseSaut, &touches);

                SDL_Rect testCarre = carre;
                if (touches.gauche) {
                    testCarre.x -= VITESSE_DEPLACEMENT;
                    if (!detecterCollision(testCarre)) carre.x = testCarre.x;
                }
                if (touches.droite) {
                    testCarre.x += VITESSE_DEPLACEMENT;
                    if (!detecterCollision(testCarre)) carre.x = testCarre.x;
                }

                if (enSaut)
                {
                    carre.y += vitesseSaut;
                    vitesseSaut += GRAVITE;

                    SDL_Rect test = carre;
                    test.y += 1;
                    if (vitesseSaut > 0 && detecterCollision(test)) {
                        carre.y = ((carre.y + carre.h) / BLOC_SIZE) * BLOC_SIZE - carre.h;
                        enSaut = SDL_FALSE;
                        vitesseSaut = 0;
                    }

                    test = carre;
                    test.y -= 1;
                    if (vitesseSaut < 0 && detecterCollision(test)) {
                        carre.y = ((carre.y) / BLOC_SIZE + 1) * BLOC_SIZE;
                        vitesseSaut = 0;
                    }
                }
                else
                {
                    SDL_Rect test = carre;
                    test.y += 1;
                    if (!detecterCollision(test)) {
                        enSaut = SDL_TRUE;
                        vitesseSaut = 0;
                    } else if (touches.saut) {
                        enSaut = SDL_TRUE;
                        vitesseSaut = FORCE_SAUT;
                        touches.saut = SDL_FALSE;
                    }
                }

                if (enSaut && sauterSurEnnemi(carre, vitesseSaut)) {
                    vitesseSaut = FORCE_SAUT / 1.5f;
                }

                mettreAJourEnnemis();
                mettreAJourEffets();

                if (finDeNiveau(carre)) {
                    scoreFinal = nbPieces;
                    etatJeu = ETAT_NIVEAU_TERMINE;
                    break;
                }

                if (detecterCollisionEnnemi(carre)) {
                    scoreGameOver = nbPieces;
                    etatJeu = ETAT_GAME_OVER;
                }

                int gauche = carre.x / BLOC_SIZE;
                int droite = (carre.x + carre.w - 1) / BLOC_SIZE;
                int haut = carre.y / BLOC_SIZE;
                int bas = (carre.y + carre.h - 1) / BLOC_SIZE;
                for (int y = haut; y <= bas; y++) {
                    for (int x = gauche; x <= droite; x++) {
                        if (x >= 0 && x < MAP_LARGEUR && y >= 0 && y < MAP_HAUTEUR && map[y][x] == BLOC_PIECE) {
                            map[y][x] = 0;
                            nbPieces++;
                        }
                    }
                }

                carre.x = SDL_clamp(carre.x, 0, MAP_LARGEUR * BLOC_SIZE - carre.w);
                cameraX = carre.x + carre.w / 2 - LONGUEUR_FENETRE / 2;
                cameraX = SDL_clamp(cameraX, 0, MAP_LARGEUR * BLOC_SIZE - LONGUEUR_FENETRE);

                SDL_SetRenderDrawColor(renderer, 129, 212, 255, 255);
                SDL_RenderClear(renderer);
                dessinerFondRepete(renderer, textures.background, cameraX);
                dessinerMap(renderer, cameraX, textures);
                dessinerEnnemis(renderer, cameraX, textures);
                dessinerEffets(renderer, cameraX);

                SDL_Rect dst = {carre.x - cameraX, carre.y, carre.w, carre.h};
                SDL_RenderCopy(renderer, textures.perso, NULL, &dst);

                afficherScore(renderer, nbPieces, police);
                calculerFPS(&fps);
                afficherFPS(renderer, &fps, police);

                SDL_RenderPresent(renderer);
                break;

            case ETAT_NIVEAU_TERMINE:
            {
                int choix = gererEvenementsNiveauTermine(&continuer, boutonsNiveauTermine, 2);
                if (current_level == NOMBRE_NIVEAUX) {
                    afficherEcranFin(renderer, police);
                    choix = ETAT_MENU;
                    current_level = 1;
                }
                if (current_level == 3)
                {
                    current_level = 4;
                    afficherMonde2(renderer, police);
                    choix = ETAT_JEU;
                }
                if (current_level == 5)
                {
                    current_level = 6;
                    afficherMonde3(renderer, police);
                    choix = ETAT_JEU;
                }
                if (choix == ETAT_JEU) {
                
                    current_level++;
                    carre.x = 100;
                    carre.y = SOL;
                    nbPieces = 0;
                    initialiserMap(current_level);
                    etatJeu = ETAT_JEU;
                } else if (choix == ETAT_MENU) {
                    etatJeu = ETAT_MENU;
                }

                SDL_SetRenderDrawColor(renderer, 0, 0, 100, 255);
                SDL_RenderClear(renderer);
                dessinerBoutons(renderer, boutonsNiveauTermine, 2, police);
                SDL_RenderPresent(renderer);
                SDL_Delay(10);
                break;
            }

            case ETAT_GAME_OVER:
            {
                SDL_Event event;
                int choix = -1;
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) continuer = SDL_FALSE;
                    else if (event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN) {
                        int x = event.motion.x, y = event.motion.y;
                        for (int i = 0; i < 2; i++) {
                            boutonsGameOver[i].hover = pointDansRect(x, y, boutonsGameOver[i].rect);
                            if (event.type == SDL_MOUSEBUTTONDOWN && boutonsGameOver[i].hover)
                                choix = i;
                        }
                    }
                }

                if (choix == 0) {
                    carre.x = 100;
                    carre.y = SOL;
                    nbPieces = 0;
                    initialiserMap(current_level);
                    etatJeu = ETAT_JEU;
                } else if (choix == 1) etatJeu = ETAT_MENU;

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);
                dessinerBoutons(renderer, boutonsGameOver, 2, police);
                SDL_RenderPresent(renderer);
                SDL_Delay(10);
                break;
            }
        }

        limiterFPS(&fps);
    }

    free(boutonsGameOver[0].texte);
    free(boutonsGameOver[1].texte);
    free(boutonsNiveauTermine[0].texte);
    free(boutonsNiveauTermine[1].texte);
    libererTextures(textures);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(fenetre);
    TTF_CloseFont(police);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
