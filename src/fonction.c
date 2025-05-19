#include "fonction.h"

//---------------------------------------------------------
// Fonction initalisation basique de SDL

SDL_Window *creerFenetre(char nom[])
{
    SDL_Window *fenetre = SDL_CreateWindow(nom, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                           LONGUEUR_FENETRE, LARGEUR_FENETRE, SDL_WINDOW_SHOWN);
    if (fenetre == NULL)
    {
        printf("Erreur SDL_CreateWindow\n");
        SDL_Quit();
        exit(1);
    }
    return fenetre;
}

SDL_Renderer *creerRenderer(SDL_Window *fenetre)
{
    SDL_Renderer *renderer = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        printf("Erreur SDL_CreateRenderer\n");
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
}

//---------------------------------------------------------
// Chargement des textures

SDL_Texture *chargerTextureBMP(SDL_Renderer *renderer, char chemin[])
{
    SDL_Surface *surface = SDL_LoadBMP(chemin);
    if (!surface)
    {
        printf("Erreur SDL_LoadBMP\n");
        exit(1);
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture)
    {
        printf("Erreur SDL_CreateTextureFromSurface\n");
        SDL_DestroyWindow(renderer);
        SDL_Quit();
        exit(1);
    }

    return texture;
}

TexturesJeu chargerTextures(SDL_Renderer *renderer)
{
    TexturesJeu textures;
    textures.bloc = chargerTextureBMP(renderer, "img/bloc.bmp");
    textures.blocSol = chargerTextureBMP(renderer, "img/sol.bmp");
    textures.blocStructure = chargerTextureBMP(renderer, "img/bloc-structure.bmp");
    textures.blocRecompense = chargerTextureBMP(renderer, "img/bloc-recompense.bmp");

    textures.tuyauHaut1 = chargerTextureBMP(renderer, "img/tuyaux-hg-1.bmp");
    textures.tuyauHaut2 = chargerTextureBMP(renderer, "img/tuyaux-hd-2.bmp");
    textures.tuyauBas1 = chargerTextureBMP(renderer, "img/tuyaux-bg-3.bmp");
    textures.tuyauBas2 = chargerTextureBMP(renderer, "img/tuyaux-bd-4.bmp");

    textures.goomba1 = chargerTextureBMP(renderer, "img/goomba-1.bmp");
    textures.goomba2 = chargerTextureBMP(renderer, "img/goomba-2.bmp");
    textures.goombaMort = chargerTextureBMP(renderer, "img/goomba-mort.bmp");
    textures.koopa1 = chargerTextureBMP(renderer, "img/tortue-1.bmp");
    textures.koopa2 = chargerTextureBMP(renderer, "img/tortue-2.bmp");
    textures.carapace1 = chargerTextureBMP(renderer, "img/carapace-1.bmp");
    textures.carapace2 = chargerTextureBMP(renderer, "img/carapace-2.bmp");

    textures.piece = chargerTextureBMP(renderer, "img/piece.bmp");
    textures.champignon = chargerTextureBMP(renderer, "img/champignon.bmp");
    textures.mario = chargerTextureBMP(renderer, "img/mario.bmp");
    textures.mario2 = chargerTextureBMP(renderer, "img/mario2.bmp");
    textures.marioBig = chargerTextureBMP(renderer, "img/mario-big.bmp");
    textures.marioBig2 = chargerTextureBMP(renderer, "img/mario-big2.bmp");
    textures.toad = chargerTextureBMP(renderer, "img/toad.bmp");

    textures.background = chargerTextureBMP(renderer, "img/fond.bmp");
    textures.vie = chargerTextureBMP(renderer, "img/vie.bmp");

    return textures;
}

void dessinerTexture(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y, int w, int h, int cameraX)
{
    SDL_Rect dest = {x * BLOC_SIZE - cameraX, y * BLOC_SIZE, w * BLOC_SIZE, h * BLOC_SIZE};
    SDL_RenderCopy(renderer, texture, NULL, &dest);
}

void libererTextures(TexturesJeu textures)
{
    if (textures.bloc)
        SDL_DestroyTexture(textures.bloc);
    if (textures.blocSol)
        SDL_DestroyTexture(textures.blocSol);
    if (textures.blocStructure)
        SDL_DestroyTexture(textures.blocStructure);
    if (textures.blocRecompense)
        SDL_DestroyTexture(textures.blocRecompense);

    if (textures.tuyauHaut1)
        SDL_DestroyTexture(textures.tuyauHaut1);
    if (textures.tuyauHaut2)
        SDL_DestroyTexture(textures.tuyauHaut2);
    if (textures.tuyauBas1)
        SDL_DestroyTexture(textures.tuyauBas1);
    if (textures.tuyauBas2)
        SDL_DestroyTexture(textures.tuyauBas2);

    if (textures.goomba1)
        SDL_DestroyTexture(textures.goomba1);
    if (textures.goomba2)
        SDL_DestroyTexture(textures.goomba2);
    if (textures.goombaMort)
        SDL_DestroyTexture(textures.goombaMort);
    if (textures.koopa1)
        SDL_DestroyTexture(textures.koopa1);
    if (textures.koopa2)
        SDL_DestroyTexture(textures.koopa2);
    if (textures.carapace1)
        SDL_DestroyTexture(textures.carapace1);
    if (textures.carapace2)
        SDL_DestroyTexture(textures.carapace2);

    if (textures.piece)
        SDL_DestroyTexture(textures.piece);
    if (textures.champignon)
        SDL_DestroyTexture(textures.champignon);
    if (textures.mario)
        SDL_DestroyTexture(textures.mario);
    if (textures.mario2)
        SDL_DestroyTexture(textures.mario2);
    if (textures.marioBig)
        SDL_DestroyTexture(textures.marioBig);
    if (textures.marioBig2)
        SDL_DestroyTexture(textures.marioBig2);
    if (textures.toad)
        SDL_DestroyTexture(textures.toad);

    if (textures.background)
        SDL_DestroyTexture(textures.background);
    if (textures.vie)
        SDL_DestroyTexture(textures.vie);
}

//---------------------------------------------------------
// Gestions des touches et des collisions (base du jeu)

void gererTouches(int *continuer, SDL_Rect *carre, int *enSaut, float *vitesseSaut, Touches *touches, SonsJeu sons)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            *continuer = 0;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_q:
                touches->gauche = 1;
                break;
            case SDLK_d:
                touches->droite = 1;
                break;
            case SDLK_SPACE:
                Mix_PlayChannel(-1, sons.saut, 0); // Jouer le son de saut
                touches->saut = 1;
                break;
            case SDLK_ESCAPE:
                *continuer = 0;
                break;
            }
            break;

        case SDL_KEYUP:
            switch (event.key.keysym.sym)
            {
            case SDLK_q:
                touches->gauche = 0;
                break;
            case SDLK_d:
                touches->droite = 0;
                break;
            }
            break;
        }
    }
}

int detecterCollision(SDL_Rect joueur)
{
    // Determiner les cotés du joueur
    int gauche = joueur.x / BLOC_SIZE;
    int droite = (joueur.x + joueur.w - 1) / BLOC_SIZE;
    int haut = joueur.y / BLOC_SIZE;
    int bas = (joueur.y + joueur.h - 1) / BLOC_SIZE;

    // Vérifier les limites de la carte
    if (gauche < 0)
        gauche = 0;
    if (droite >= MAP_LARGEUR)
        droite = MAP_LARGEUR - 1;
    if (haut < 0)
        haut = 0;
    if (bas >= MAP_HAUTEUR)
        bas = MAP_HAUTEUR - 1;

    // Vérifier les collisions avec les blocs
    for (int y = haut; y <= bas; y++)
    {
        for (int x = gauche; x <= droite; x++)
        {
            if (x < 0 || x >= MAP_LARGEUR || y < 0 || y >= MAP_HAUTEUR)
                continue;

            int bloc = map[y][x];

            // On ignore les pièces et TOAD
            if (bloc != 0 && bloc != PIECE && bloc != TOAD)
            {
                // Si c'est un bloc mystère, vérifier si Mario le touche par dessous
                if (bloc == BLOC_RECOMPENSE)
                {
                    int hautBloc = y * BLOC_SIZE;
                    int basJoueur = joueur.y + joueur.h;

                    // Collision par en dessous > on ne bloque pas
                    if (basJoueur <= hautBloc + 5)
                        continue;
                }

                return 1; // Collision détectée
            }
        }
    }

    return 0; // Pas de collision
}

//---------------------------------------------------------
// Initialisation et affichage des maps et des ennemis

// Initalisation des maps dans le fichier map.c

int map[MAP_HAUTEUR][MAP_LARGEUR]; // Map de jeu

void initialiserMap(int niveau)
{
    char (*sourceMap)[MAP_LARGEUR];

    // On selectionne la map en fonction du niveau
    switch (niveau)
    {
    case 1:
        sourceMap = niveau1;
        break;
    case 2:
        sourceMap = niveau2;
        break;
    case 3:
        sourceMap = niveau3;
        break;
    case 4:
        sourceMap = niveau4;
        break;
    case 5:
        sourceMap = niveau5;
        break;
    case 6:
        sourceMap = niveau6;
        break;
    case 7:
        sourceMap = niveau7;
        break;
    case 8:
        sourceMap = niveau8;
        break;
    case 9:
        sourceMap = niveau9;
        break;
    case 0:
        sourceMap = bonus;
        break;
    default:
        sourceMap = niveau1;
        break;
    }

    // Initialiser la map à 0
    for (int y = 0; y < MAP_HAUTEUR; y++)
    {
        for (int x = 0; x < MAP_LARGEUR; x++)
        {
            map[y][x] = 0;
        }
    }

    // Remplir la map avec les blocs correspondants
    for (int y = 0; y < MAP_HAUTEUR; y++)
    {
        for (int x = 0; x < MAP_LARGEUR; x++)
        {
            switch (sourceMap[y][x])
            {
            case '#':
                map[y][x] = BLOC;
                break;
            case '=':
                map[y][x] = BLOC_SOL;
                break;
            case '$':
                map[y][x] = PIECE;
                break;
            case '?':
                map[y][x] = BLOC_RECOMPENSE;
                break;
            case '1':
                map[y][x] = TUYAU_HAUT_GAUCHE;
                break;
            case '2':
                map[y][x] = TUYAU_HAUT_DROITE;
                break;
            case '3':
                map[y][x] = TUYAU_BAS_GAUCHE;
                break;
            case '4':
                map[y][x] = TUYAU_BAS_DROITE;
                break;
            case 'f':
                map[y][x] = TOAD;
                break;
            case 'u':
                map[y][x] = BLOC_STRUCTURE;
                break;
            default:
                map[y][x] = 0;
                break;
            }
        }
    }

    // Initialiser les ennemis
    initialiserEnnemis(sourceMap);
}

Ennemi ennemis[MAX_ENNEMIS]; // Tableau d'ennemis

void initialiserEnnemis(char niveau[][MAP_LARGEUR])
{
    // Initialiser les ennemis à inactif
    for (int i = 0; i < MAX_ENNEMIS; i++)
    {
        ennemis[i].actif = 0;
    }

    int nombreEnnemis = 0;

    for (int y = 0; y < MAP_HAUTEUR; y++)
    {
        for (int x = 0; x < MAP_LARGEUR; x++)
        {
            if (nombreEnnemis >= MAX_ENNEMIS)
                return;

            if (niveau[y][x] == 'g') // Goomba
            {
                ennemis[nombreEnnemis].rect.x = x * BLOC_SIZE;
                ennemis[nombreEnnemis].rect.y = y * BLOC_SIZE;
                ennemis[nombreEnnemis].rect.w = BLOC_SIZE;
                ennemis[nombreEnnemis].rect.h = BLOC_SIZE;
                ennemis[nombreEnnemis].direction = (rand() % 2) ? DROITE : GAUCHE; // Direction aléatoire entre droite et gauche (On a appris la technique du ? avec 1 ou 0 pour faire un choix ou un autre)
                ennemis[nombreEnnemis].type = GOOMBA;
                ennemis[nombreEnnemis].actif = 1;
                nombreEnnemis++;
            }

            if (niveau[y][x] == 'k') // Koopa
            {
                ennemis[nombreEnnemis].rect.x = x * BLOC_SIZE;
                ennemis[nombreEnnemis].rect.y = y * BLOC_SIZE;
                ennemis[nombreEnnemis].rect.w = BLOC_SIZE;
                ennemis[nombreEnnemis].rect.h = BLOC_SIZE;
                ennemis[nombreEnnemis].direction = (rand() % 2) ? DROITE : GAUCHE;
                ennemis[nombreEnnemis].type = KOOPA;
                ennemis[nombreEnnemis].actif = 1;
                nombreEnnemis++;
            }
        }
    }
}

void dessinerMap(SDL_Renderer *renderer, int cameraX, TexturesJeu textures)
{
    for (int y = 0; y < MAP_HAUTEUR; y++)
    {
        for (int x = 0; x < MAP_LARGEUR; x++)
        {
            int bloc = map[y][x];
            if (bloc == 0)
                continue;

            SDL_Texture *texture = NULL;

            switch (bloc)
            {
            case BLOC:
                texture = textures.bloc;
                break;
            case BLOC_SOL:
                texture = textures.blocSol;
                break;
            case BLOC_STRUCTURE:
                texture = textures.blocStructure;
                break;
            case BLOC_RECOMPENSE:
                texture = textures.blocRecompense;
                break;
            case TUYAU_BAS_DROITE:
                texture = textures.tuyauBas2;
                break;
            case TUYAU_BAS_GAUCHE:
                texture = textures.tuyauBas1;
                break;
            case TUYAU_HAUT_DROITE:
                texture = textures.tuyauHaut2;
                break;
            case TUYAU_HAUT_GAUCHE:
                texture = textures.tuyauHaut1;
                break;
            case PIECE:
                texture = textures.piece;
                break;
            case TOAD:
                texture = textures.toad;
                break;
            default:
                texture = NULL;
                break;
            }

            if (texture)
                dessinerTexture(renderer, texture, x, y, 1, 1, cameraX);
        }
    }
}

void dessinerEnnemis(SDL_Renderer *renderer, int cameraX, TexturesJeu textures)
{
    for (int i = 0; i < MAX_ENNEMIS; i++)
    {
        if (ennemis[i].actif)
        {
            SDL_Rect actuel = {
                ennemis[i].rect.x - cameraX,
                ennemis[i].rect.y,
                ennemis[i].rect.w,
                ennemis[i].rect.h};

            SDL_Texture *texture = NULL;

            if (ennemis[i].type == GOOMBA)
                texture = ennemis[i].animation ? textures.goomba1 : textures.goomba2;
            else if (ennemis[i].type == KOOPA)
                texture = ennemis[i].animation ? textures.koopa1 : textures.koopa2;

            if (texture)
                SDL_RenderCopy(renderer, texture, NULL, &actuel);
        }
    }
}

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

//---------------------------------------------------------
// Affichage du score / vies

void afficherScore(SDL_Renderer *renderer, ScoreJeu *scoreJeu, TTF_Font *police)
{
    char texte[64];
    sprintf(texte, "Score : %d", scoreJeu->score);

    SDL_Color couleur = {255, 255, 255};
    SDL_Surface *surfaceTexte = TTF_RenderText_Solid(police, texte, couleur);
    if (!surfaceTexte)
    {
        printf("Erreur TTF_RenderText_Solid\n");
        return;
    }

    SDL_Texture *textureTexte = SDL_CreateTextureFromSurface(renderer, surfaceTexte);
    if (!textureTexte)
    {
        printf("Erreur SDL_CreateTextureFromSurface\n");
        SDL_FreeSurface(surfaceTexte);
        return;
    }

    SDL_Rect dest = {10, 10, surfaceTexte->w, surfaceTexte->h};
    SDL_RenderCopy(renderer, textureTexte, NULL, &dest);

    SDL_FreeSurface(surfaceTexte);
    SDL_DestroyTexture(textureTexte);
}

void afficherVies(SDL_Renderer *renderer, ScoreJeu *scoreJeu, TexturesJeu textures)
{
    int taille = 30;
    int marge = 10;
    SDL_Rect rect;
    rect.y = 10;
    rect.w = taille;
    rect.h = taille;

    // Aligné à droite
    rect.x = LONGUEUR_FENETRE - scoreJeu->vies * (taille + marge);

    for (int i = 0; i < scoreJeu->vies; i++)
    {
        SDL_RenderCopy(renderer, textures.vie, NULL, &rect);
        rect.x += taille + marge;
    }
}

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

//---------------------------------------------------------
// Menu

int gererEvenementsMenu(int *continuer, Bouton boutons[], int nombreBoutons)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            *continuer = 0;
            break;

        case SDL_MOUSEMOTION:
        {
            int mx = event.motion.x, my = event.motion.y;
            for (int i = 0; i < nombreBoutons; i++)
                boutons[i].hover = pointDansRect(mx, my, boutons[i].rect);
        }
        break;

        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                int mx = event.button.x, my = event.button.y;
                for (int i = 0; i < nombreBoutons; i++)
                {
                    if (pointDansRect(mx, my, boutons[i].rect))
                    {
                        return i;
                    }
                }
            }
            break;

        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
                *continuer = 0;
            break;
        }
    }
    return -1;
}

//---------------------------------------------------------
// Gestion transition entre les niveaux

int finDeNiveau(SDL_Rect joueur)
{
    int joueurGaucheBloc = (joueur.x / BLOC_SIZE > 0) ? (joueur.x / BLOC_SIZE - 1) : 0;
    int joueurDroiteBloc = ((joueur.x + joueur.w - 1) / BLOC_SIZE + 1 < MAP_LARGEUR) ? ((joueur.x + joueur.w - 1) / BLOC_SIZE + 1) : (MAP_LARGEUR - 1);
    int joueurHautBloc = (joueur.y / BLOC_SIZE > 0) ? (joueur.y / BLOC_SIZE - 1) : 0;
    int joueurBasBloc = ((joueur.y + joueur.h - 1) / BLOC_SIZE + 1 < MAP_HAUTEUR) ? ((joueur.y + joueur.h - 1) / BLOC_SIZE + 1) : (MAP_HAUTEUR - 1);

    for (int y = joueurHautBloc; y <= joueurBasBloc; y++)
    {
        for (int x = joueurGaucheBloc; x <= joueurDroiteBloc; x++)
        {

            if (map[y][x] == TOAD)
            {
                // Calculer les coordonnées réelles du bloc de fin
                SDL_Rect blocFin = {
                    x * BLOC_SIZE,
                    y * BLOC_SIZE,
                    BLOC_SIZE,
                    BLOC_SIZE};

                if (joueur.x < blocFin.x + blocFin.w &&
                    joueur.x + joueur.w > blocFin.x &&
                    joueur.y < blocFin.y + blocFin.h &&
                    joueur.y + joueur.h > blocFin.y)
                {
                    return 1;
                }
            }
        }
    }

    return 0;
}

int gererEvenementsNiveauTermine(int *continuer, Bouton boutons[], int nombreBoutons)
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            *continuer = 0;
            break;

        case SDL_MOUSEMOTION:
        {
            int mouseX = event.motion.x;
            int mouseY = event.motion.y;

            for (int i = 0; i < nombreBoutons; i++)
            {
                if (mouseX >= boutons[i].rect.x && mouseX <= boutons[i].rect.x + boutons[i].rect.w &&
                    mouseY >= boutons[i].rect.y && mouseY <= boutons[i].rect.y + boutons[i].rect.h)
                {
                    boutons[i].hover = 1;
                }
                else
                {
                    boutons[i].hover = 0;
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
                            return ETAT_JEU;
                        else if (i == 1)
                            return ETAT_MENU;
                    }
                }
            }
            break;

        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                return ETAT_MENU;
            }
            break;
        }
    }

    return -1;
}

int gererGameOver(int *continuer, Bouton boutons[], int nombreBoutons)
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            *continuer = 0;
            break;

        case SDL_MOUSEMOTION:
        {
            int mouseX = event.motion.x;
            int mouseY = event.motion.y;

            for (int i = 0; i < nombreBoutons; i++)
            {
                if (mouseX >= boutons[i].rect.x && mouseX <= boutons[i].rect.x + boutons[i].rect.w &&
                    mouseY >= boutons[i].rect.y && mouseY <= boutons[i].rect.y + boutons[i].rect.h)
                {
                    boutons[i].hover = 1;
                }
                else
                {
                    boutons[i].hover = 0;
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
                            return ETAT_JEU;
                        else if (i == 1)
                            return ETAT_MENU;
                    }
                }
            }
            break;

        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                return ETAT_MENU;
            }
            break;
        }
    }

    return -1;
}

//---------------------------------------------------------
// Affichage des transitions entre les mondes

void afficherMonde2(SDL_Renderer *renderer, TTF_Font *police)
{
    SDL_Color couleur = {255, 255, 255};
    SDL_Surface *surfaceTexte = TTF_RenderText_Solid(police, "Monde 2", couleur);
    if (!surfaceTexte)
    {
        printf("Erreur TTF_RenderText_Solid\n");
        return;
    }

    SDL_Texture *textureTexte = SDL_CreateTextureFromSurface(renderer, surfaceTexte);
    SDL_FreeSurface(surfaceTexte);
    if (!textureTexte)
    {
        printf("Erreur SDL_CreateTextureFromSurface\n");
        return;
    }

    SDL_Rect rectTexte = {
        (LONGUEUR_FENETRE - surfaceTexte->w) / 2,
        (LARGEUR_FENETRE - surfaceTexte->h) / 2,
        surfaceTexte->w,
        surfaceTexte->h};

    int continuer = 1;
    SDL_Event event;

    while (continuer)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN)
            {
                continuer = 0;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, textureTexte, NULL, &rectTexte);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyTexture(textureTexte);
}

void afficherMonde3(SDL_Renderer *renderer, TTF_Font *police)
{
    SDL_Color couleur = {255, 255, 255};
    SDL_Surface *surfaceTexte = TTF_RenderText_Solid(police, "Monde 3", couleur);
    if (!surfaceTexte)
    {
        printf("Erreur TTF_RenderText_Solid\n");
        return;
    }

    SDL_Texture *textureTexte = SDL_CreateTextureFromSurface(renderer, surfaceTexte);
    SDL_FreeSurface(surfaceTexte);
    if (!textureTexte)
    {
        printf("Erreur SDL_CreateTextureFromSurface\n");
        return;
    }

    SDL_Rect rectTexte = {
        (LONGUEUR_FENETRE - surfaceTexte->w) / 2,
        (LARGEUR_FENETRE - surfaceTexte->h) / 2,
        surfaceTexte->w,
        surfaceTexte->h};

    int continuer = 1;
    SDL_Event event;

    while (continuer)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN)
            {
                continuer = 0;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, textureTexte, NULL, &rectTexte);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyTexture(textureTexte);
}

void afficherEcranFin(SDL_Renderer *renderer, TTF_Font *police)
{
    SDL_Color couleur = {255, 255, 255};
    SDL_Surface *surfaceTexte = TTF_RenderText_Solid(police, "Bien joue ! Jeu termine !", couleur);
    if (!surfaceTexte)
    {
        printf("Erreur TTF_RenderText_Solid\n");
        return;
    }

    SDL_Texture *textureTexte = SDL_CreateTextureFromSurface(renderer, surfaceTexte);
    SDL_FreeSurface(surfaceTexte);
    if (!textureTexte)
    {
        printf("Erreur SDL_CreateTextureFromSurface\n");
        return;
    }

    SDL_Rect rectTexte = {
        (LONGUEUR_FENETRE - surfaceTexte->w) / 2,
        (LARGEUR_FENETRE - surfaceTexte->h) / 2,
        surfaceTexte->w,
        surfaceTexte->h};

    int continuer = 1;
    SDL_Event event;

    while (continuer)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN)
            {
                continuer = 0;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, textureTexte, NULL, &rectTexte);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyTexture(textureTexte);
}

//---------------------------------------------------------
// Affichage background

void dessinerFondParallaxe(SDL_Renderer *renderer, SDL_Texture *texture, int cameraX)
{
    int vitesseParallaxe = 3;
    int offsetX = cameraX / vitesseParallaxe;

    SDL_Rect dst = {-offsetX, 0, LONGUEUR_FENETRE, LARGEUR_FENETRE};
    while (dst.x < LONGUEUR_FENETRE)
    {
        SDL_RenderCopy(renderer, texture, NULL, &dst);
        dst.x += LONGUEUR_FENETRE;
    }
}

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

void afficherTableauScores(SDL_Renderer *renderer, TTF_Font *police)
{
    Sauvegarde sauvegardes[MAX_SAUVEGARDES];
    int nb = chargerToutesLesSauvegardes(sauvegardes, MAX_SAUVEGARDES);

    if (nb == 0)
        return;

    // Tri décroissant des scores
    for (int i = 0; i < nb - 1; i++)
    {
        for (int j = i + 1; j < nb; j++)
        {
            if (sauvegardes[j].scoreMax > sauvegardes[i].scoreMax)
            {
                Sauvegarde tmp = sauvegardes[i];
                sauvegardes[i] = sauvegardes[j];
                sauvegardes[j] = tmp;
            }
        }
    }

    int afficher = (nb < 10) ? nb : 10;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0.5); // fond semi-transparent
    SDL_Rect fond = {LONGUEUR_FENETRE / 2 - 250, 100, 500, 60 + afficher * 40};
    SDL_RenderFillRect(renderer, &fond);

    SDL_Color couleurTexte = {255, 255, 255};
    SDL_Surface *titre = TTF_RenderText_Solid(police, "CLASSEMENT DES SCORES", couleurTexte);
    SDL_Texture *titreTex = SDL_CreateTextureFromSurface(renderer, titre);
    SDL_Rect posTitre = {fond.x + (fond.w - titre->w) / 2, fond.y + 10, titre->w, titre->h};
    SDL_RenderCopy(renderer, titreTex, NULL, &posTitre);
    SDL_FreeSurface(titre);
    SDL_DestroyTexture(titreTex);

    for (int i = 0; i < afficher; i++)
    {
        char ligne[128];
        snprintf(ligne, sizeof(ligne), "%2d. %-20s  %5d pts", i + 1, sauvegardes[i].nom, sauvegardes[i].scoreMax);

        SDL_Surface *surface = TTF_RenderText_Solid(police, ligne, couleurTexte);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect pos = {
            fond.x + 30,
            fond.y + 50 + i * 35,
            surface->w,
            surface->h};
        SDL_RenderCopy(renderer, texture, NULL, &pos);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }

    SDL_RenderPresent(renderer);

    SDL_Event ev;
    int attendre = 1;
    while (attendre)
    {
        while (SDL_PollEvent(&ev))
        {
            if (ev.type == SDL_QUIT)
            {
                attendre = 0;
            }
            else if (ev.type == SDL_KEYDOWN || ev.type == SDL_MOUSEBUTTONDOWN)
            {
                attendre = 0;
            }
        }
        SDL_Delay(10);
    }
}

//---------------------------------------------------------
// SDL mixer

SonsJeu chargerSons()
{
    SonsJeu sons;

    sons.saut = Mix_LoadWAV("sons/saut.wav");
    sons.piece = Mix_LoadWAV("sons/piece.wav");
    sons.powerUp = Mix_LoadWAV("sons/power-up.wav");
    sons.niveauTermine = Mix_LoadWAV("sons/niveau-termine.wav");
    sons.mondeTermine = Mix_LoadWAV("sons/monde-termine.wav");
    sons.gameOver = Mix_LoadWAV("sons/game-over.wav");
    sons.musiqueFond = Mix_LoadMUS("sons/theme-song.wav");

    if (!sons.saut || !sons.piece || !sons.powerUp || !sons.niveauTermine ||
        !sons.mondeTermine || !sons.gameOver || !sons.musiqueFond)
    {
        printf("Erreur chargement sons\n");
        exit(1);
    }

    return sons;
}

void libererSons(SonsJeu sons)
{
    Mix_FreeChunk(sons.saut);
    Mix_FreeChunk(sons.piece);
    Mix_FreeChunk(sons.powerUp);
    Mix_FreeChunk(sons.niveauTermine);
    Mix_FreeChunk(sons.mondeTermine);
    Mix_FreeChunk(sons.gameOver);
    Mix_FreeMusic(sons.musiqueFond);
}