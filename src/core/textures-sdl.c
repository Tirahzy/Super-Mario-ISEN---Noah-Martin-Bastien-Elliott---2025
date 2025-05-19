#include "../fonction.h"

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
