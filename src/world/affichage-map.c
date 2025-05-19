#include "../fonction.h"

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