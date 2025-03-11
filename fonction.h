#ifndef FONCTION_H
#define FONCTION_H
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <SDL2/SDL.h>

typedef struct {
    int x;
    int y;
    int w;
    int h;
} Mario;

int affichageMenu(int argc, char* argv[]);

#endif //FONCTION_H
