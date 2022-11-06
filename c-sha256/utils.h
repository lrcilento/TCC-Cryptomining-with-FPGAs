#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int number0;
    int number1;
}
tuple;

typedef struct
{
    int number0;
    int number1;
    int number2;
}
thraay;

tuple *zip(int x[], int y[]);
thraay *zipThree(int x[], int y[], int z[]);
int isTrue(int x);
int if_(int i, int y, int z);
int and_(int i, int j);
int *AND(int x[], int y[]);
int not_(int i);
int *NOT(int x[]);
int xor_(int i, int j);
int *XOR(int x[], int y[]);
int xorxor(int i, int j, int l);
int *XORXOR(int x[], int y[], int z[]);
int maj(int i, int j, int k);
int *rotr(int x[], int size, int n);
int *shr(int x[], int size, int n);
int *add(int x[], int y[], int size);