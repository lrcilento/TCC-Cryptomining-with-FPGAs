#include "constants.h"
#include "helpers.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

// SHA-256 main function
void *sha256(char *message, int *output) {
    // It initializes the values of "K" (array of hexadecimal constants) by converting them into arrays of bits
    int k[64][32];
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 32; j++) {
            k[i][j] = *(initializer(K[i]) + j);
        }
    }
    // It initializes the values of "H_HEX" (array of hexadecimal constants) by converting them into arrays of bits
    int h[8][32];
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 32; j++) {
            h[i][j] = *(initializer(H_HEX[i]) + j);
        }
    }
    // Returns an array of 512 bits
    int *chunks = preprocessMessage(message);
    // Resizes the array
    int resizedChunks[(512 + (48 * 32))];
    for (int i = 0; i < (512 + (48 * 32)); i++) {
        if (i < 512) {
            resizedChunks[i] = chunks[i];
        } else {
            resizedChunks[i] = 48;
        }
    }
    // MOVE THIS TO THE KERNEL
    // CHUNKER FUNCTION LOGIC
    int numRows = (512 + (48 * 32)) / 32;
    int chunked[numRows][32];
    int counter = 0;
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < 32; j++) {
            chunked[i][j] = resizedChunks[counter];
            counter++;
        }
    }
    // Logic to mix everything to turn the input in a hash
    for (int i = 16; i < 64; i++) {
        int *s0 = XORXOR(rotr(chunked[i - 15], 32, 7), rotr(chunked[i - 15], 32, 18), shr(chunked[i - 15], 32, 3));
        int *s1 = XORXOR(rotr(chunked[i - 2], 32, 17), rotr(chunked[i - 2], 32, 19), shr(chunked[i - 2], 32, 10));
        int *helper = add(add(add(chunked[i - 16], s0, 32), chunked[i - 7], 32), s1, 32);
        for (int j = 0; j < 32; j++) {
            chunked[i][j] = helper[j];
        }
    }
    int *a = h[0];
    int *b = h[1];
    int *c = h[2];
    int *d = h[3];
    int *e = h[4];
    int *f = h[5];
    int *g = h[6];
    int *i = h[7];
    // More mixing
    for (int j = 0; j < 64; j++) {
        int *S1 = XORXOR(rotr(e, 32, 6), rotr(e, 32, 11), rotr(e, 32, 25));
        int *ch = XOR(AND(e, f), AND(NOT(e), g));
        int *temp1 = add(add(add(add(i, S1, 32), ch, 32), k[j], 32), chunked[j], 32);
        int *S0 = XORXOR(rotr(a, 32, 2), rotr(a, 32, 13), rotr(a, 32, 22));
        int *m = XORXOR(AND(a, b), AND(a, c), AND(b, c));
        int *temp2 = add(S0, m, 32);
        i = g;
        g = f;
        f = e;
        e = add(d, temp1, 32);
        d = c;
        c = b;
        b = a;
        a = add(temp1, temp2, 32);
    }
    int *aux0 = add(h[0], a, 32);
    for (int j = 0; j < 32; j++) {
        h[0][j] = aux0[j];
    }
    int *aux1 = add(h[1], b, 32);
    for (int j = 0; j < 32; j++) {
        h[1][j] = aux1[j];
    }
    int *aux2 = add(h[2], c, 32);
    for (int j = 0; j < 32; j++) {
        h[2][j] = aux2[j];
    }
    int *aux3 = add(h[3], d, 32);
    for (int j = 0; j < 32; j++) {
        h[3][j] = aux3[j];
    }
    int *aux4 = add(h[4], e, 32);
    for (int j = 0; j < 32; j++) {
        h[4][j] = aux4[j];
    }
    int *aux5 = add(h[5], f, 32);
    for (int j = 0; j < 32; j++) {
        h[5][j] = aux5[j];
    }
    int *aux6 = add(h[6], g, 32);
    for (int j = 0; j < 32; j++) {
        h[6][j] = aux6[j];
    }
    int *aux7 = add(h[7], i, 32);
    for (int j = 0; j < 32; j++) {
        h[7][j] = aux7[j];
    }
    int index = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 32; j++) {
            output[index] = h[i][j];
            index++;
        }
    }
}

int main(void) {
    // Gets the user input
    char *inputMessage = malloc(55 * sizeof(char));
    printf("Type a text: ");
    scanf("%s", inputMessage);

    char *workingMessage = malloc(55 * sizeof(char));
    strcpy(workingMessage, inputMessage);

    char *magicNumber;
    int *output = malloc((8 * 32) * sizeof(int));
    int zeros, done = 1, add = 0;
    printf("Type the required amount of zeros: ");
    scanf("%d", &zeros);

    double start = clock();
    // Calls the sha256 function
    // Prints the final hash output in binary
    while(1) {
        sha256(workingMessage, output);
        for(int i = 0; i < zeros; i++) {
            if(output[i] != '0') {
                done = 0;
                break;
            }
        }
        if(done == 1) {
            break;
        }
        done = 1;
        add = add + 1;
        sprintf(magicNumber, "%d", add);
        strcpy(workingMessage, inputMessage);
        strcpy(workingMessage, strcat(workingMessage, magicNumber));
    }
    double end = clock();
    double kernel_time = (double)(end - start)/CLOCKS_PER_SEC;

    printf("Hash: ");
    for (int i = 0; i < 8 * 32; i++) {
        printf("%c", output[i]);
    }
    printf("\nMagic Number: ");
    printf("%d", add);
    printf("\nTime: ");
    printf("%f", kernel_time);
    printf("\n");

    free(workingMessage);
    free(inputMessage);
    free(output);
}
