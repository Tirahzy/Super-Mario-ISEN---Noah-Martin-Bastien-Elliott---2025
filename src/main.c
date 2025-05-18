#include "fonction.h"

int main(int argc, char *argv[])
{
    srand(time(NULL));

    // Initialisation de SDL, TTF et Mix
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Erreur SDL_Init\n");
        return 1;
    }

    if (TTF_Init() == -1)
    {
        printf("Erreur TTF_Init\n");
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("Erreur Mix_OpenAudio\n");
        return 1;
    }

    TTF_Font *police = TTF_OpenFont("fonts/arial.ttf", 24);
    if (!police)
    {
        printf("Erreur chargement police\n");
        SDL_Quit();
        return 1;
    }

    // Création de la fenêtre et du renderer
    SDL_Window *fenetre = creerFenetre("Super Mario ISEN");
    SDL_Renderer *renderer = creerRenderer(fenetre);

    // Chargement des textures et sons
    TexturesJeu textures = chargerTextures(renderer);
    SonsJeu sons = chargerSons();
    Mix_Volume(-1, MIX_MAX_VOLUME / 10); // Diminue le volume
    int musiqueEnCours = 0;

    // Initialisation des variables de jeu
    int startX = 100; // Position de départ de Mario X
    int startY = (MAP_HAUTEUR - 3) * BLOC_SIZE - BLOC_SIZE; // Position de départ de Mario Y
    Mario mario = {{startX, startY, BLOC_SIZE, BLOC_SIZE}, 0};

    Touches touches = {0, 0, 0}; // Aucune touche enfoncée
    int enSaut = 0; // Indique si Mario est en train de sauter
    float vitesseSaut = 0.0f;
    int cameraX = 0;
    int scoreFinal = 0; // Score final du joueur
    int current_level = 1; // Niveau actuel
    char nomUtilisateur[50];
    Sauvegarde data; // Données de sauvegarde
    int continuer = 1; // Variable pour la boucle principale
    int etatJeu = ETAT_MENU; // État du jeu (menu, jeu, game over, etc.)
    ScoreJeu scoreJeu = {0, 3}; // Score et vies du joueur

    initialiserMap(current_level);
    initialiserCarapaces();
    initialiserEffets();
    Champignon champi = {{0, 0, BLOC_SIZE, BLOC_SIZE}, 0, 0.0f};

    // Création des boutons
    char *labelsMenu[NB_BOUTONS_MENU] = {
        "Jouer",
        "Niveau",
        "Classement",
        "Quitter"};
    Bouton boutonsMenu[NB_BOUTONS_MENU];
    initialiserBoutons(boutonsMenu, NB_BOUTONS_MENU, labelsMenu);

    Bouton boutonsNiveauTermine[2] = {
        {{380, 200, 200, 50}, strdup("Niveau Suivant"), 0},
        {{380, 270, 200, 50}, strdup("Menu Principal"), 0}};

    Bouton boutonsGameOver[2] = {
        {{380, 200, 200, 50}, strdup("Rejouer"), 0},
        {{380, 270, 200, 50}, strdup("Menu Principal"), 0}};

    // Recupération de l'utilisateur
    int chargementReussi = 0;
    do
    {
        saisirNomUtilisateur(renderer, police, nomUtilisateur, sizeof(nomUtilisateur));

        if (chargerUtilisateur(nomUtilisateur, &data))
        {
            fflush(stdout); // On a trouvé une sauvegarde

            int choix = afficherChoixChargement(renderer, police, fenetre);

            fflush(stdout); // On a affiché le choix de chargement

            if (choix == 0)
            { // Charger la sauvegarde
                current_level = data.niveauActuel;
                scoreJeu.score = data.score;
                chargementReussi = 1;
            }
            else if (choix == -1)
            {
                // Fermer le jeu (fermeture fenêtre SDL)
                continuer = 0;
                break;
            }
        }
        else
        {
            // Nouveau joueur

            fflush(stdout);
            current_level = 1;
            scoreJeu.score = 0;
            data.niveauActuel = current_level;
            data.niveauMax = current_level;
            data.score = scoreJeu.score;
            sauvegarderUtilisateur(nomUtilisateur, data.niveauActuel, data.score);

            chargementReussi = 1;
        }
    } while (!chargementReussi && continuer);

    // Détermine et charge le bon background
    SDL_DestroyTexture(textures.background);
    if (current_level == 0)
        textures.background = chargerTextureBMP(renderer, "img/bonus.bmp");
    else if (current_level <= 3)
        textures.background = chargerTextureBMP(renderer, "img/fond.bmp");
    else if (current_level <= 6)
        textures.background = chargerTextureBMP(renderer, "img/fond2.bmp");
    else
        textures.background = chargerTextureBMP(renderer, "img/fond3.bmp");

    while (continuer)
    {
        switch (etatJeu)
        {
        case ETAT_MENU:
        {
            // Couper la musique si elle est en cours
            if (musiqueEnCours)
            {
                Mix_HaltMusic();
                musiqueEnCours = 0;
            }

            int choix = gererEvenementsMenu(&continuer, boutonsMenu, NB_BOUTONS_MENU);

            if (choix == 0) // Jouer
            {
                etatJeu = ETAT_JEU;

                // Lancer la musique de fond
                if (!musiqueEnCours)
                {
                    Mix_PlayMusic(sons.musiqueFond, -1); // jouer en boucle
                    musiqueEnCours = 1;
                }

                mario.invincible = 0;
                mario.tempsInvincible = 0;

                mario.corps.x = startX;
                mario.corps.y = startY;
                mario.estGrand = 0;
                mario.corps.h = BLOC_SIZE;
                champi.actif = 0;
                champi.vitesseY = 0;
                champi.corps.x = 0;
                champi.corps.y = 0;
                scoreJeu.vies = 3;

                initialiserMap(current_level);
                SDL_DestroyTexture(textures.background);
                if (current_level == 0)
                    textures.background = chargerTextureBMP(renderer, "img/bonus.bmp");
                else if (current_level <= 3)
                    textures.background = chargerTextureBMP(renderer, "img/fond.bmp");
                else if (current_level <= 6)
                    textures.background = chargerTextureBMP(renderer, "img/fond2.bmp");
                else
                    textures.background = chargerTextureBMP(renderer, "img/fond3.bmp");

                initialiserCarapaces();
                initialiserEffets();
            }
            else if (choix == 1) // Sélection niveau
            {
                etatJeu = ETAT_SELECTION;
            }
            else if (choix == 2) // Classement
            {
                afficherTableauScores(renderer, police);
                etatJeu = ETAT_MENU;
                continue;
            }
            else if (choix == 3) // Quitter
            {
                continuer = 0;
            }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            dessinerBoutons(renderer, boutonsMenu, NB_BOUTONS_MENU, police);
            SDL_RenderPresent(renderer);
            SDL_Delay(16);
            break;
        }

        case ETAT_SELECTION:
        {
            static const char *labelsNiv[NOMBRE_NIVEAUX] = {
                "Niveau 1", "Niveau 2", "Niveau 3",
                "Niveau 4", "Niveau 5", "Niveau 6",
                "Niveau 7", "Niveau 8", "Niveau 9"};

            Bouton boutonsSelect[NOMBRE_NIVEAUX];
            int largeur = 180, hauteur = 50, espacement = 20;
            int debutX = (LONGUEUR_FENETRE - (3 * largeur + 2 * espacement)) / 2;
            int debutY = 100;

            for (int i = 0; i < NOMBRE_NIVEAUX; i++)
            {
                boutonsSelect[i].rect.x = debutX + (i % 3) * (largeur + espacement);
                boutonsSelect[i].rect.y = debutY + (i / 3) * (hauteur + 50);
                boutonsSelect[i].rect.w = largeur;
                boutonsSelect[i].rect.h = hauteur;
                boutonsSelect[i].texte = labelsNiv[i];
                // Grise les niveaux non atteints
                boutonsSelect[i].hover = (i + 1 > data.niveauMax) ? -1 : 0;
            }

            int choixNiv = -1;
            SDL_Event ev;
            int quitterSelection = 0;

            while (choixNiv < 0 && continuer && !quitterSelection)
            {
                while (SDL_PollEvent(&ev))
                {
                    if (ev.type == SDL_QUIT)
                        continuer = 0;

                    else if (ev.type == SDL_MOUSEMOTION)
                    {
                        int mx = ev.motion.x, my = ev.motion.y;
                        for (int i = 0; i < NOMBRE_NIVEAUX; i++)
                        {
                            if (i + 1 <= data.niveauMax)
                                boutonsSelect[i].hover = pointDansRect(mx, my, boutonsSelect[i].rect);
                            else
                                boutonsSelect[i].hover = -1;
                        }
                    }

                    else if (ev.type == SDL_MOUSEBUTTONDOWN && ev.button.button == SDL_BUTTON_LEFT)
                    {
                        int mx = ev.button.x, my = ev.button.y;
                        for (int i = 0; i < NOMBRE_NIVEAUX; i++)
                        {
                            if (pointDansRect(mx, my, boutonsSelect[i].rect) && boutonsSelect[i].hover != -1)
                            {
                                choixNiv = i + 1;
                                break;
                            }
                        }
                    }

                    else if (ev.type == SDL_KEYDOWN)
                    {
                        if (ev.key.keysym.sym == SDLK_ESCAPE)
                        {
                            quitterSelection = 1;
                            break;
                        }
                        else if (ev.key.keysym.sym == SDLK_g)
                        {
                            choixNiv = 0; // Bonus
                            break;
                        }
                    }
                }

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);

                SDL_Color couleur = {255, 255, 255};
                for (int ligne = 0; ligne < 3; ligne++)
                {
                    char texteMonde[32];
                    sprintf(texteMonde, "Monde %d :", ligne + 1);
                    SDL_Surface *surface = TTF_RenderText_Solid(police, texteMonde, couleur);
                    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
                    SDL_Rect pos = {debutX - 150, debutY + ligne * (hauteur + 50) + 10, surface->w, surface->h};
                    SDL_RenderCopy(renderer, texture, NULL, &pos);
                    SDL_FreeSurface(surface);
                    SDL_DestroyTexture(texture);
                }

                dessinerBoutons(renderer, boutonsSelect, NOMBRE_NIVEAUX, police);
                SDL_RenderPresent(renderer);
                SDL_Delay(16);
            }

            if ((choixNiv > 0 && choixNiv <= data.niveauMax) || choixNiv == 0)
            {
                current_level = choixNiv;
                mario.corps.x = startX;
                mario.corps.y = startY;
                scoreJeu.score = 0;

                SDL_DestroyTexture(textures.background);
                if (current_level == 0)
                    textures.background = chargerTextureBMP(renderer, "img/bonus.bmp");
                else if (current_level <= 3)
                    textures.background = chargerTextureBMP(renderer, "img/fond.bmp");
                else if (current_level <= 6)
                    textures.background = chargerTextureBMP(renderer, "img/fond2.bmp");
                else
                    textures.background = chargerTextureBMP(renderer, "img/fond3.bmp");

                initialiserMap(current_level);
                initialiserCarapaces();
                initialiserEffets();
                etatJeu = ETAT_JEU;

                // Lancer la musique de fond
                if (!musiqueEnCours)
                {
                    Mix_PlayMusic(sons.musiqueFond, -1); // Jouer en boucle
                    musiqueEnCours = 1;
                }
            }
            else if (quitterSelection)
            {
                etatJeu = ETAT_MENU;
            }

            break;
        }

        case ETAT_JEU:
        {
            gererTouches(&continuer, &mario.corps, &enSaut, &vitesseSaut, &touches, sons);

            // Déplacement de Mario
            SDL_Rect test = mario.corps;
            if (touches.gauche)
            {
                test.x -= VITESSE_DEPLACEMENT;
                if (!detecterCollision(test))
                    mario.corps.x = test.x;
                    mario.direction = GAUCHE;
            }
            if (touches.droite)
            {
                test.x += VITESSE_DEPLACEMENT;
                if (!detecterCollision(test))
                    mario.corps.x = test.x;
                    mario.direction = DROITE;
            }

            if (enSaut)
            {
                mario.corps.y += vitesseSaut;
                vitesseSaut += GRAVITE;

                test = mario.corps;
                test.y += 2;
                if (vitesseSaut > 0 && detecterCollision(test))
                {
                    mario.corps.y = (mario.corps.y + mario.corps.h) / BLOC_SIZE * BLOC_SIZE - mario.corps.h;
                    vitesseSaut = 0;
                    enSaut = 0;
                }

                test = mario.corps;
                test.y -= 1;
                if (vitesseSaut < 0 && detecterCollisionBlocMystere(mario.corps, vitesseSaut))
                {
                    int xBloc = (mario.corps.x + mario.corps.w / 2) / BLOC_SIZE;
                    int yBloc = (mario.corps.y - 1) / BLOC_SIZE;

                    if (map[yBloc][xBloc] == BLOC_RECOMPENSE)
                    {
                        map[yBloc][xBloc] = BLOC_STRUCTURE;
                        if (!mario.estGrand)
                        {
                            champi.corps.x = xBloc * BLOC_SIZE;
                            champi.corps.y = (yBloc - 1) * BLOC_SIZE - 5; // anti-glitch

                            champi.vitesseY = -2;
                            champi.vitesseX = 3.5f; // ou 2.0f pour + rapide
                            champi.direction = 1;   // droite par défaut
                            champi.actif = 1;
                        }
                        else
                        {
                            map[yBloc - 1][xBloc] = PIECE;
                        }
                    }

                    int yDessousBloc = (mario.corps.y - 1) / BLOC_SIZE;
                    mario.corps.y = (yDessousBloc + 1) * BLOC_SIZE;
                    vitesseSaut = 0;
                }
                else if (vitesseSaut < 0 && detecterCollision(test))
                {
                    int yDessousBloc = (mario.corps.y - 1) / BLOC_SIZE;
                    mario.corps.y = (yDessousBloc + 1) * BLOC_SIZE;
                    vitesseSaut = 0;
                }
            }
            else
            {
                test = mario.corps;
                test.y += 8;
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

            // Vérifier si Mario est sur un ennemi
            if (enSaut && sauterSurEnnemi(mario.corps, vitesseSaut, &scoreJeu))
                vitesseSaut = FORCE_SAUT / 1.5f; // Réduit la vitesse de saut après avoir sauté sur un ennemi

            if (!mario.invincible && interagirAvecCarapaces(&mario.corps, &vitesseSaut))
            {
                if (mario.estGrand)
                {
                    // Simule le corps réduit
                    SDL_Rect futurCorps = mario.corps;
                    futurCorps.h = BLOC_SIZE;
                    futurCorps.y += (mario.corps.h - BLOC_SIZE);

                    // Check si y'a de la place
                    if (!detecterCollision(futurCorps))
                    {
                        mario.corps = futurCorps;
                    }
                    else
                    {
                        // Pas de place → on pousse encore plus (juste en dessous)
                        mario.corps.y += BLOC_SIZE;
                        mario.corps.h = BLOC_SIZE;
                    }

                    mario.estGrand = 0;
                    mario.invincible = 1;
                    mario.tempsInvincible = SDL_GetTicks();
                }
                else
                {
                    scoreJeu.vies--;
                    if (scoreJeu.vies <= 0)
                    {
                        scoreFinal = scoreJeu.score;
                        etatJeu = ETAT_GAME_OVER;
                        break;
                    }
                    else
                    {
                        // Réinitialisation du niveau sans perdre le score
                        mario.corps.x = startX;
                        mario.corps.y = startY;
                        enSaut = 0;
                        vitesseSaut = 0;
                        touches.gauche = touches.droite = touches.saut = 0;
                        initialiserMap(current_level);
                        SDL_DestroyTexture(textures.background);
                        if (current_level == 0)
                            textures.background = chargerTextureBMP(renderer, "img/bonus.bmp");
                        else if (current_level <= 3)
                            textures.background = chargerTextureBMP(renderer, "img/fond.bmp");
                        else if (current_level <= 6)
                            textures.background = chargerTextureBMP(renderer, "img/fond2.bmp");
                        else
                            textures.background = chargerTextureBMP(renderer, "img/fond3.bmp");

                        initialiserCarapaces();
                        initialiserEffets();
                        cameraX = 0;
                        break;
                    }
                }
            }

            carapacesTuantEnnemis();
            mettreAJourCarapaces();
            mettreAJourEnnemis();
            mettreAJourEffets();

            // Gérer l'invincibilité
            if (mario.invincible)
            {
                if (SDL_GetTicks() - mario.tempsInvincible >= 1000) // SDL_GetTicks retourne le temps écoulé depuis le lancement du programme
                {
                    mario.invincible = 0;
                }
            }

            if (!mario.invincible && (detecterCollisionEnnemi(mario.corps) || mario.corps.y > MAP_HAUTEUR * BLOC_SIZE))
            {
                if (mario.estGrand)
                {
                    // Préparer futur corps de Mario petit
                    SDL_Rect futurCorps = mario.corps;
                    futurCorps.h = BLOC_SIZE;
                    futurCorps.y += (mario.corps.h - BLOC_SIZE); // Décale vers le bas

                    if (!detecterCollision(futurCorps))
                    {
                        mario.corps = futurCorps;
                    }
                    else
                    {
                        // Il y a un bloc en dessous, on pousse encore plus pour pas se bloquer
                        mario.corps.y += BLOC_SIZE;
                        mario.corps.h = BLOC_SIZE;
                    }

                    mario.estGrand = 0;
                    mario.invincible = 1;
                    mario.tempsInvincible = SDL_GetTicks();
                }
                else
                {
                    scoreJeu.vies--;
                    if (scoreJeu.vies <= 0)
                    {
                        scoreFinal = scoreJeu.score;
                        etatJeu = ETAT_GAME_OVER;
                        break;
                    }
                    else
                    {
                        // Réinitialisation du niveau sans perdre le score
                        mario.corps.x = startX;
                        mario.corps.y = startY;
                        enSaut = 0;
                        vitesseSaut = 0;
                        touches.gauche = touches.droite = touches.saut = 0;
                        initialiserMap(current_level);
                        SDL_DestroyTexture(textures.background);
                        if (current_level == 0)
                            textures.background = chargerTextureBMP(renderer, "img/bonus.bmp");
                        else if (current_level <= 3)
                            textures.background = chargerTextureBMP(renderer, "img/fond.bmp");
                        else if (current_level <= 6)
                            textures.background = chargerTextureBMP(renderer, "img/fond2.bmp");
                        else
                            textures.background = chargerTextureBMP(renderer, "img/fond3.bmp");

                        // Recharger le niveau actuel
                        initialiserCarapaces();
                        initialiserEffets();
                        cameraX = 0;
                        break;
                    }
                }
            }

            int gauche = mario.corps.x / BLOC_SIZE;
            int droite = (mario.corps.x + mario.corps.w - 1) / BLOC_SIZE;
            int haut = mario.corps.y / BLOC_SIZE;
            int bas = (mario.corps.y + mario.corps.h - 1) / BLOC_SIZE;

            for (int y = haut; y <= bas; y++)
                for (int x = gauche; x <= droite; x++)
                    if (map[y][x] == PIECE)
                    {
                        map[y][x] = 0;
                        Mix_PlayChannel(-1, sons.piece, 0);
                        scoreJeu.score += 500;
                    }

            if (finDeNiveau(mario.corps))
            {
                scoreJeu.score += 2000;
                etatJeu = ETAT_NIVEAU_TERMINE;
                break;
            }

            if (champi.actif)
            {
                // GRAVITÉ
                champi.vitesseY += GRAVITE;
                SDL_Rect testY = champi.corps;
                testY.y += (int)champi.vitesseY;

                if (!detecterCollision(testY))
                {
                    champi.corps.y = testY.y;
                }
                else
                {
                    champi.vitesseY = 0;
                    // Ajuste pile sur le bloc, sans le repousser violemment car sinon rebond
                    champi.corps.y = (champi.corps.y + BLOC_SIZE) / BLOC_SIZE * BLOC_SIZE - champi.corps.h;
                }

                // DÉPLACEMENT HORIZONTAL
                SDL_Rect testX = champi.corps;
                testX.x += champi.vitesseX * champi.direction;

                if (!detecterCollision(testX))
                {
                    champi.corps.x = testX.x;
                }
                else
                {
                    champi.direction *= -1;
                }
                if (SDL_HasIntersection(&champi.corps, &mario.corps))
                {
                    champi.actif = 0;
                    Mix_PlayChannel(-1, sons.powerUp, 0);
                    if (!mario.estGrand)
                    {
                        mario.estGrand = 1;
                        mario.corps.h = BLOC_SIZE * 1.75;
                        mario.corps.y -= BLOC_SIZE;
                    }
                }
            }

            // SDL_clamp sert à limiter la valeur de cameraX entre 0 et MAP_LARGEUR * BLOC_SIZE - LONGUEUR_FENETRE
            mario.corps.x = SDL_clamp(mario.corps.x, 0, MAP_LARGEUR * BLOC_SIZE - mario.corps.w);
            cameraX = mario.corps.x + mario.corps.w / 2 - LONGUEUR_FENETRE / 2;
            cameraX = SDL_clamp(cameraX, 0, MAP_LARGEUR * BLOC_SIZE - LONGUEUR_FENETRE);

            SDL_SetRenderDrawColor(renderer, 129, 212, 255, 255);
            SDL_RenderClear(renderer);
            dessinerFondParallaxe(renderer, textures.background, cameraX);
            dessinerMap(renderer, cameraX, textures);
            dessinerEnnemis(renderer, cameraX, textures);
            dessinerCarapaces(renderer, cameraX, textures);
            dessinerEffets(renderer, cameraX, textures);

            SDL_Rect dst = {mario.corps.x - cameraX, mario.corps.y, mario.corps.w, mario.corps.h};
            SDL_Texture *textureMario = (mario.direction == GAUCHE) ? textures.mario2 : textures.mario;
            SDL_Texture *textureMarioBig = (mario.direction == GAUCHE) ? textures.marioBig2 : textures.marioBig;
            SDL_RenderCopy(renderer, mario.estGrand ? textureMarioBig : textureMario, NULL, &dst);

            if (champi.actif)
            {
                SDL_Rect dstCh = {champi.corps.x - cameraX, champi.corps.y, champi.corps.w, champi.corps.h};
                SDL_RenderCopy(renderer, textures.champignon, NULL, &dstCh);
            }

            afficherScore(renderer, &scoreJeu, police);
            afficherVies(renderer, &scoreJeu, textures);
            SDL_RenderPresent(renderer);
            SDL_Delay(16);
            break;
        }

        case ETAT_NIVEAU_TERMINE:
        {
            touches.gauche = touches.droite = touches.saut = 0;
            int choix = gererEvenementsNiveauTermine(&continuer, boutonsNiveauTermine, 2);

            if (current_level == NOMBRE_NIVEAUX)
            {
                if (musiqueEnCours)
                {
                    Mix_HaltMusic();
                    musiqueEnCours = 0;
                }
                Mix_PlayChannel(-1, sons.mondeTermine, 0); // Son de fin de monde
                afficherEcranFin(renderer, police);
                choix = ETAT_MENU;
                textures.background = chargerTextureBMP(renderer, "img/fond.bmp");
                current_level = 1;
            }
            else if (current_level == 3)
            {
                if (musiqueEnCours)
                {
                    Mix_HaltMusic();
                    musiqueEnCours = 0;
                }
                Mix_PlayChannel(-1, sons.mondeTermine, 0);
                afficherMonde2(renderer, police);
                SDL_Delay(2000);
                textures.background = chargerTextureBMP(renderer, "img/fond2.bmp");
                choix = ETAT_JEU;
            }
            else if (current_level == 6)
            {
                if (musiqueEnCours)
                {
                    Mix_HaltMusic();
                    musiqueEnCours = 0;
                }
                Mix_PlayChannel(-1, sons.mondeTermine, 0);
                afficherMonde3(renderer, police);
                SDL_Delay(2000);
                textures.background = chargerTextureBMP(renderer, "img/fond3.bmp");
                choix = ETAT_JEU;
            }
            else
            {
                if (musiqueEnCours)
                {
                    Mix_HaltMusic();
                    musiqueEnCours = 0;
                }
                Mix_PlayChannel(-1, sons.niveauTermine, 0); // Son de niveau terminé
            }

            if (choix == ETAT_JEU)
            {
                if (!musiqueEnCours)
                {
                    Mix_PlayMusic(sons.musiqueFond, -1); // jouer en boucle
                    musiqueEnCours = 1;
                }
                // Si on ne vient pas du niveau bonus, on incrémente et sauvegarde la progression
                if (current_level != 0)
                {
                    current_level++;

                    // Mise à jour du niveau maximum si nécessaire
                    if (current_level > data.niveauMax)
                        data.niveauMax = current_level;

                    data.niveauActuel = current_level;
                    data.score = scoreJeu.score;

                    sauvegarderUtilisateur(nomUtilisateur, data.niveauActuel, data.score);
                }
                else
                {
                    // Niveau bonus terminé, pas de progression sauvegardée
                    current_level = 1; // Repartir du niveau 1
                }

                mario.corps.x = startX;
                mario.corps.y = startY;
                scoreJeu.vies = 3;

                initialiserMap(current_level);

                SDL_DestroyTexture(textures.background);
                if (current_level == 0)
                    textures.background = chargerTextureBMP(renderer, "img/bonus.bmp");
                else if (current_level <= 3)
                    textures.background = chargerTextureBMP(renderer, "img/fond.bmp");
                else if (current_level <= 6)
                    textures.background = chargerTextureBMP(renderer, "img/fond2.bmp");
                else
                    textures.background = chargerTextureBMP(renderer, "img/fond3.bmp");

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
            Mix_HaltMusic();
            if (musiqueEnCours)
            {
                Mix_HaltMusic();
                musiqueEnCours = 0;
            }
            Mix_PlayChannel(-1, sons.gameOver, 0);
            
            mario.invincible = 0;
            mario.tempsInvincible = 0;
            champi.actif = 0;
            champi.vitesseY = 0;
            champi.corps.x = 0;
            champi.corps.y = 0;

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
                current_level = 1;
                textures.background = chargerTextureBMP(renderer, "img/fond.bmp");
                mario.corps
                    .x = startX;
                mario.corps
                    .y = startY;
                scoreJeu.score = 0;
                scoreJeu.vies = 3;
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
                if (!musiqueEnCours)
                {
                    Mix_PlayMusic(sons.musiqueFond, -1); // Jouer en boucle
                    musiqueEnCours = 1;
                }
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

    // Sauvegarde avant de quitter
    data.niveauActuel = current_level;
    data.score = scoreJeu.score;
    if (current_level != 0)
        sauvegarderUtilisateur(nomUtilisateur, current_level, scoreJeu.score);

    // Libération des ressources
    free(boutonsNiveauTermine[0].texte);
    free(boutonsNiveauTermine[1].texte);
    free(boutonsGameOver[0].texte);
    free(boutonsGameOver[1].texte);
    libererTextures(textures);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(fenetre);
    TTF_CloseFont(police);
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
    return 0;
}