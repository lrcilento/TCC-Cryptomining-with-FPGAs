#include "../host/inc/constants.h"
#include "../host/inc/helpers.h"

__attribute__((max_global_work_dim(0)))
__kernel void stmi(__global char* inputMessage, int zeros, __global char* k, __global char* h) {
	int *chunks = preprocessMessage(inputMessage);
	int resizedChunks[(512 + (48 * 32))];
    for (int i = 0; i < (512 + (48 * 32)); i++) {
        if (i < 512) {
            resizedChunks[i] = chunks[i];
        } else {
            resizedChunks[i] = 48;
        }
    }
	int numRows = (512 + (48 * 32)) / 32;
    int chunked[numRows][32];
    int counter = 0;
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < 32; j++) {
            chunked[i][j] = resizedChunks[counter];
            counter++;
        }
    }
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
    int *output = (int *) malloc((8 * 32) * sizeof(int));
    int index = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 32; j++) {
            output[index] = h[i][j];
            index++;
        }
    }
    inputMessage = output;
}
