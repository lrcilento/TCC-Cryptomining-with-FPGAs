#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *bin(int input);
char *zfill(char *strInput, int intInput);
int *translate(char *message);
int *chunker(int *bits, int bitsLenght, int chunkLenght);
int *fillZeros(int *bits, int bitsLenght, int desiredLenght, char *endian);
int *preprocessMessage(char *message);
int *initializer(char *value);