#include "../fonction.h"

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