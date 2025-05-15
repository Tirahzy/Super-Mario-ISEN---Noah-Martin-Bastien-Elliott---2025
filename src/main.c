#include "fonction.h"

// Commit Elliott

int main(int argc, char *argv[])
{
    srand(time(NULL));

    // Initialisation de SDL et TTF
    if (SDL_Init(SDL_INIT_VIDEO) != 0 || TTF_Init() != 0)
    {
        fprintf(stderr, "Erreur SDL ou TTF_Init : %s\n", SDL_GetError());
        return 1;
    }

    // Chargement de la police
    TTF_Font *police = TTF_OpenFont("fonts/arial.ttf", 24);
    if (!police)
    {
        fprintf(stderr, "Erreur chargement police : %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    // Création de la fenêtre et du renderer
    SDL_Window *fenetre = creerFenetre("Super Mario ISEN");
    SDL_Renderer *renderer = creerRenderer(fenetre);

    // Chargement des textures
    TexturesJeu textures = chargerTextures(renderer);

    // Position de départ de Mario
    int startX = 100;
    int startY = (MAP_HAUTEUR - 3) * BLOC_SIZE - BLOC_SIZE;
    SDL_Rect joueur = { startX, startY, BLOC_SIZE, BLOC_SIZE };

    // Initialisation des variables de jeu
    Touches touches = {0, 0, 0};
    int enSaut = 0;
    float vitesseSaut = 0.0f;
    int cameraX = 0;
    int nbPieces = 0;
    int scoreFinal = 0;
    int current_level = 1;
    int continuer = 1;
    int etatJeu = ETAT_MENU;

    // Initialisation des maps, ennemis, carapaces et effets
    initialiserMap(current_level);
    initialiserCarapaces();
    initialiserEffets();

    // Boutons pour les menus
    Bouton boutonsMenu[2];
    initialiserBoutons(boutonsMenu, 2);

    Bouton boutonsNiveauTermine[2] = {
        {{380, 200, 200, 50}, strdup("Niveau Suivant"), 0},
        {{380, 270, 200, 50}, strdup("Menu Principal"), 0}
    };

    Bouton boutonsGameOver[2] = {
        {{380, 200, 200, 50}, strdup("Rejouer"), 0},
        {{380, 270, 200, 50}, strdup("Menu Principal"), 0}
    };

    // Boucle principale
    while (continuer)
    {
        switch (etatJeu)
        {
            case ETAT_MENU:
            {
                int choix = gererEvenementsMenu(&continuer, boutonsMenu, 2);
                if (choix == ETAT_JEU)
                {
                    // Lancement du premier niveau
                    current_level = 1;
                    joueur.x = startX;
                    joueur.y = startY;
                    nbPieces = 0;
                    initialiserMap(current_level);
                    initialiserCarapaces();
                    initialiserEffets();
                    etatJeu = ETAT_JEU;
                }
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);
                dessinerBoutons(renderer, boutonsMenu, 2, police);
                SDL_RenderPresent(renderer);
                SDL_Delay(16);
                break;
            }

            case ETAT_JEU:
            {
                // Gestion des touches
                gererTouches(&continuer, &joueur, &enSaut, &vitesseSaut, &touches);

                // Déplacement horizontal
                SDL_Rect test = joueur;
                if (touches.gauche)
                {
                    test.x -= VITESSE_DEPLACEMENT;
                    if (!detecterCollision(test)) joueur.x = test.x;
                }
                if (touches.droite)
                {
                    test.x += VITESSE_DEPLACEMENT;
                    if (!detecterCollision(test)) joueur.x = test.x;
                }

                // Gestion du saut et de la gravité
                if (enSaut)
                {
                    joueur.y += vitesseSaut;
                    vitesseSaut += GRAVITE;

                    // Collision en descendant
                    test = joueur;
                    test.y += 1;
                    if (vitesseSaut > 0 && detecterCollision(test))
                    {
                        joueur.y = ((joueur.y + joueur.h) / BLOC_SIZE) * BLOC_SIZE - joueur.h;
                        enSaut = 0;
                        vitesseSaut = 0;
                    }

                    // Collision en montant
                    test = joueur;
                    test.y -= 1;
                    if (vitesseSaut < 0 && detecterCollision(test))
                    {
                        joueur.y = ((joueur.y) / BLOC_SIZE + 1) * BLOC_SIZE;
                        vitesseSaut = 0;
                    }
                }
                else
                {
                    // Si plus de sol sous Mario, il tombe
                    test = joueur;
                    test.y += 1;
                    if (!detecterCollision(test))
                    {
                        enSaut = 1;
                        vitesseSaut = 0;
                    }
                    else if (touches.saut)
                    {
                        enSaut = 1;
                        vitesseSaut = FORCE_SAUT;
                        touches.saut = 0;
                    }
                }

                // Interaction avec ennemis et carapaces
                if (enSaut && sauterSurEnnemi(joueur, vitesseSaut))
                {
                    vitesseSaut = FORCE_SAUT / 1.5f;
                }
                if (interagirAvecCarapaces(&joueur, &vitesseSaut))
                {
                    scoreFinal = nbPieces;
                    etatJeu = ETAT_GAME_OVER;
                    break;
                }

                // Mise à jour des entités
                carapacesTuantEnnemis();
                mettreAJourCarapaces();
                mettreAJourEnnemis();
                mettreAJourEffets();

                // Collision ennemis
                if (detecterCollisionEnnemi(joueur) || joueur.y > MAP_HAUTEUR * BLOC_SIZE)
                {
                    scoreFinal = nbPieces;
                    etatJeu = ETAT_GAME_OVER;
                    break;
                }

                // Ramassage des pièces
                int gauche = joueur.x / BLOC_SIZE;
                int droite = (joueur.x + joueur.w - 1) / BLOC_SIZE;
                int haut = joueur.y / BLOC_SIZE;
                int bas = (joueur.y + joueur.h - 1) / BLOC_SIZE;
                for (int y = haut; y <= bas; y++)
                    for (int x = gauche; x <= droite; x++)
                        if (map[y][x] == PIECE)
                        {
                            map[y][x] = 0;
                            nbPieces++;
                        }

                // Passage de niveau
                if (finDeNiveau(joueur))
                {
                    scoreFinal = nbPieces;
                    etatJeu = ETAT_NIVEAU_TERMINE;
                    break;
                }

                // Caméra
                joueur.x = SDL_clamp(joueur.x, 0, MAP_LARGEUR * BLOC_SIZE - joueur.w);
                cameraX = joueur.x + joueur.w / 2 - LONGUEUR_FENETRE / 2;
                cameraX = SDL_clamp(cameraX, 0, MAP_LARGEUR * BLOC_SIZE - LONGUEUR_FENETRE);

                // Rendu
                SDL_SetRenderDrawColor(renderer, 129, 212, 255, 255);
                SDL_RenderClear(renderer);
                dessinerFondParallaxe(renderer, textures.background, cameraX);
                dessinerMap(renderer, cameraX, textures);
                dessinerEnnemis(renderer, cameraX, textures);
                dessinerCarapaces(renderer, cameraX, textures);
                dessinerEffets(renderer, cameraX, textures);
                SDL_Rect dst = { joueur.x - cameraX, joueur.y, joueur.w, joueur.h };
                SDL_RenderCopy(renderer, textures.mario, NULL, &dst);
                afficherScore(renderer, nbPieces, police);
                SDL_RenderPresent(renderer);
                SDL_Delay(16);
                break;
            }

            case ETAT_NIVEAU_TERMINE:
            {
                // Réinitialisation des touches
                touches.gauche = touches.droite = touches.saut = 0;
                int choix = gererEvenementsNiveauTermine(&continuer, boutonsNiveauTermine, 2);

                if (current_level == NOMBRE_NIVEAUX)
                {
                    afficherEcranFin(renderer, police);
                    choix = ETAT_MENU;
                    textures.background = chargerTextureBMP(renderer, "img/fond.bmp");
                    current_level = 1;
                }
                else if (current_level == 3)
                {
                    afficherMonde2(renderer, police);
                    textures.background = chargerTextureBMP(renderer, "img/fond2.bmp");
                    choix = ETAT_JEU;
                }
                else if (current_level == 6)
                {
                    afficherMonde3(renderer, police);
                    textures.background = chargerTextureBMP(renderer, "img/fond3.bmp");
                    choix = ETAT_JEU;
                }

                if (choix == ETAT_JEU)
                {
                    current_level++;
                    joueur.x = startX;
                    joueur.y = startY;
                    nbPieces = 0;
                    initialiserMap(current_level);
                    initialiserCarapaces();
                    initialiserEffets();
                    etatJeu = ETAT_JEU;
                }
                else if (choix == ETAT_MENU)
                {
                    etatJeu = ETAT_MENU;
                }

                SDL_SetRenderDrawColor(renderer, 0, 0, 100, 255);
                SDL_RenderClear(renderer);
                dessinerBoutons(renderer, boutonsNiveauTermine, 2, police);
                SDL_RenderPresent(renderer);
                SDL_Delay(16);
                break;
            }

            case ETAT_GAME_OVER:
            {
                touches.gauche = touches.droite = touches.saut = 0;
                SDL_Event event;
                int choix = -1;

                while (SDL_PollEvent(&event))
                {
                    if (event.type == SDL_QUIT)
                        continuer = 0;
                    else if (event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN)
                    {
                        int x = event.motion.x;
                        int y = event.motion.y;
                        for (int i = 0; i < 2; i++)
                        {
                            boutonsGameOver[i].hover = pointDansRect(x, y, boutonsGameOver[i].rect);
                            if (event.type == SDL_MOUSEBUTTONDOWN && boutonsGameOver[i].hover)
                                choix = i;
                        }
                    }
                }

                if (choix == 0)
                {
                    // Rejouer
                    joueur.x = startX;
                    joueur.y = startY;
                    nbPieces = 0;
                    enSaut = 0;
                    vitesseSaut = 0;
                    touches.gauche = touches.droite = touches.saut = 0;
                    initialiserMap(current_level);
                    initialiserCarapaces();
                    initialiserEffets();
                    for (int i = 0; i < MAX_ENNEMIS; i++)
                    {
                        carapaces[i].mobile = 0;
                        carapaces[i].direction = 0;
                        carapaces[i].tempsLancement = 0;
                    }
                    etatJeu = ETAT_JEU;
                }
                else if (choix == 1)
                {
                    etatJeu = ETAT_MENU;
                }

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);
                dessinerBoutons(renderer, boutonsGameOver, 2, police);
                SDL_RenderPresent(renderer);
                SDL_Delay(16);
                break;
            }
        }
    }

    // Libération des ressources
    free(boutonsNiveauTermine[0].texte);
    free(boutonsNiveauTermine[1].texte);
    free(boutonsGameOver[0].texte);
    free(boutonsGameOver[1].texte);
    libererTextures(textures);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(fenetre);
    TTF_CloseFont(police);
    TTF_Quit();
    SDL_Quit();
    return 0;
}