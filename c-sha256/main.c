#include "constants.h"
#include "helpers.h"
#include "utils.h"

// SHA-256 main function
char *sha256(char *message) {
    // It initializes the values of "K" (array of hexadecimal constants) by converting them into arrays of bits
    int k[64][32];
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 32; j++) {
            k[i][j] = *(initializer(K[i]) + j);
        }
    }

    // printf("%c", k[1][2]);

    int h[8][32];
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 32; j++) {
            h[i][j] = *(initializer(H_HEX[i]) + j);
        }
    }

    // printf("%c", h[1][3]);

    int messageLenght = strlen(message) * 8;
    int *chunks = preprocessMessage(message);

    int resizedChunks[(512 + (48 * 32)) * sizeof(int)];
    for (int i = 0; i < (512 + (48 * 32)); i++) {
        if (i < 512) {
            resizedChunks[i] = chunks[i];
        } else {
            resizedChunks[i] = 48;
        }
        
        // printf("%c", chunks[i]);
    }
    // printf("\n--------------------------------------------------\n");
    // int *w = chunker(chunks, 512, 32);

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

    // for (int i = 0; i < numRows; i++) {
    //     for (int j = 0; j < 32; j++) {
    //         printf("%c", chunked[i][j]);
    //     }
    //     printf("\n---------------------------\n");
    // }

    // TODO

    // int *teste = malloc(32 * sizeof(int));
    // for (int j = 0; j < 32; j++) {
    //     teste[] = chunked[16 - 15][j]);
    //     // printf("%c", chunked[16 - 15][j]);
    // }


    // printf("%lu", sizeof(chunked[1]));
    int *test = XORXOR(rotr(chunked[1], 32, 7), rotr(chunked[1], 32, 18), shr(chunked[1], 32, 3));
    for (int s = 0; s < 32; s++) {
        printf("%c", test[s]);
    }

    // for (int i = 16; i < 64; i++) {
    //     XORXOR(rotr(chunked[i - 15], 32, 7), rotr(chunked[i - 15], 32, 18), shr(chunked[i - 15], 32, 3));
    // }

    return "output";
}

int main(void) {
    // Gets the user input
    char inputMessage[55];
    printf("Type a text: ");
    scanf("%s", inputMessage);
    // Calls the sha256 function
    // Prints the final hash output
    printf("Hash: %s\n", sha256(inputMessage));
}