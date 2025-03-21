CC = gcc
CFLAGS = -Wall -I include
LDFLAGS = -L lib -lmingw32 -lSDL2main -lSDL2 -mwindows

Programme : main.o fonction.o
	$(CC) main.o fonction.o -o Jeu $(LDFLAGS)

main.o : src/main.c src/fonction.h
	$(CC) $(CFLAGS) -c src/main.c -o main.o

fonction.o : src/fonction.c src/fonction.h
	$(CC) $(CFLAGS) -c src/fonction.c -o fonction.o

# Suppression des fichiers temporaires.
clean :
	del -rf *.o

# Suppression de tous les fichiers, sauf les sources,
# en vue d’une reconstruction complète.
mrproper : clean
	del Programme