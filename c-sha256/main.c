#include "constants.h"
#include "helpers.h"
#include <stdio.h>
#include <string.h>

// SHA-256 main function
char *sha256(char *message) {
    // It initializes the values of "K" (array of hexadecimal constants) by converting them into arrays of bits
    // for (int i = 0; i < 64; i++) {
    //     printf("%s\n", K[i]);
    // }
    int *k = initializer(K, 64);
    // FIX THIS RETURN
    // printf("%p\n", &k);
    // printf("%p\n", &k + 1);
    // printf("%p\n", &k - 1);
    // printf("\n-------------------------\n");
    // printf("%c\n", *(*(&k)));

    // printf("%c\n", *(*(k)) + 1);
    // for (int i = 0; i < 64; i++) {
    //     for (int j = 0; j < 32; j++) {
    //         printf("%c", *(&(k + i) + j));
    //     }
    // }
    return "output";
}

int main(void) {
    // Gets the user input
    char *inputMessage;
    printf("Type a text: ");
    scanf("%s", inputMessage);
    // Calls the sha256 function
    // Prints the final hash output
    printf("Hash: %s\n", sha256(inputMessage));
}