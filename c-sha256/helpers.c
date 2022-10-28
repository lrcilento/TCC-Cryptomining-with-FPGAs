#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// It receives an integer input and returns a binary string
char *bin(int input) {
    // If the input is == 0, it returns a hardcoded string
    if (input == 0) {
        return "0b0";
    }
    // It counts how much memory will be necessary to allocate 
    int firstCounter = 0;
    int temp = input;
    while (temp > 0) {
        temp = temp / 2;
        firstCounter++;
    }
    // It does the logic to convert the integer input into a binary string
    char *binaryString = malloc(firstCounter);
    int secondCounter = 0;
    for (int i = 0; i < firstCounter; i++) {
        char string[sizeof(int)];
        int digit = input % 2;
        sprintf(string, "%d", digit);
        *(binaryString + secondCounter) = string[0];
        input /= 2;
        secondCounter++;
    }
    // It does the logic to add the "0b" prefix and invert the binary string
    char *invertedBinaryString = malloc(firstCounter + 2);
    *invertedBinaryString = *"0";
    *(invertedBinaryString + 1) = *"b";
    for (int i = 2; i < firstCounter + 2; i++) {
        invertedBinaryString[i] = binaryString[secondCounter - 1];
        secondCounter--;
    }
    // It deallocates the memory
    free(binaryString);
    // It returns the binary string
    return invertedBinaryString;
}

// It fills the beggining of the string input with 0's until it has the same length as the integer input
char *zfill(char *strInput, int intInput) {
    char *strOutput = malloc(intInput);
    int zerosToAdd = intInput - strlen(strInput);
    // It does the logic to add 0's to the beggining of the string
    int counter = 0;
    for (int i = 0; i < intInput; i++) {
        if (i >= zerosToAdd) {
            *(strOutput + i) = strInput[counter];
            counter++;
        } else {
            *(strOutput + i) = *"0";
        }
    }
    // Returns the function output
    return strOutput;
}

// It returns the string input as an array of bits output (8 bits for every character in the input)
int *translate(char *message) {
    // It takes the Unicode code point of every character in the message and inserts in an array
    char charCodes[strlen(message)];
    for (int i = 0; i < strlen(message); i++) {
        charCodes[i] = *(message + i);
    }
    // It converts every Unicode code point to binary and chops off the '0b' binary indicator
    char *bytes[strlen(message)];
    int counter = 0;
    for (int i = 0; i < sizeof(charCodes); i++) {
        char *tempCode = bin(*(charCodes + i));
        char *code = malloc(strlen(tempCode));
        
        for (int j = 0; j < strlen(tempCode); j++) {
            if (j >= 2) {
                code[counter] = *(tempCode + j);
                counter++;
            }
        }
        counter = 0;

        bytes[i] = zfill(code, 8);
        free(code);
    }
    // It takes the binary strings and turns them into a list, every bit in a different index
    int *bits = malloc(strlen(message) * 8);
    for (int i = 0; i < sizeof(bytes) / 8; i++) {
        for (int j = 0; j < strlen(bytes[i]); j++) {
            bits[counter] = bytes[i][j];
            counter++;
        }
    }
    // It returns a single list containing all the bits of the input characters
    return bits;
}

// It receives as an input an array of bits and a chunk lenght
// It breaks the array of bits in chunks of bits and returns a pinter to an array containing these arrays of chunks of bits
int *chunker(int *bits, int chunk_lenght) {
    // It counts how much memory will be necessary to allocate
    int sizeToAllocate = 0;
    int sizeToAllocateAux = 0;    
    if ((*(&bits + 1) - bits) > chunk_lenght) {
        for (int i = 0; i < *(&bits + 1) - bits; i++) {
            sizeToAllocateAux++;
            if (sizeToAllocateAux == chunk_lenght) {
                sizeToAllocate++;
                sizeToAllocateAux = 0;
            }
        }
        if (sizeToAllocateAux > 0) {
            sizeToAllocate++;
        }
    } else {
        sizeToAllocate = 1;
    }
    int *chunked[sizeToAllocate];
    // It allocates the memory
    int chunkedSize = sizeToAllocate;
    for (int i = 0; i < chunkedSize; i++) {
        if ((i == chunkedSize - 1) && (sizeToAllocateAux > 0)) {
            chunked[i] = malloc(sizeToAllocateAux);
        } else if ((*(&bits + 1) - bits) < chunk_lenght) {
            chunked[i] = malloc(*(&bits + 1) - bits);
        } else {
            chunked[i] = malloc(chunk_lenght);
        }
    }
    // It points every "row/array" of the chunked array to the input data it has to have
    int counter = 0;
    for (int i = 0; i < chunkedSize; i++) {
        chunked[i] = &bits[counter];
        counter += chunk_lenght;
    }
    
    // // DEBUG
    // // It prints the data of every "row/array" of the chunked array
    // int k = 0;
    // for (int i = 0; i < chunkedSize; i++) {
  
    //     int *p = chunked[i];
    //     if (i == chunkedSize - 1 && sizeToAllocateAux > 0) {
    //         for (int j = 0; j < sizeToAllocateAux; j++) {
    //             printf("%d ", *p);
    //             // Move the pointer to the next element
    //             p++;
    //         }
    //     } else {
    //         for (int j = 0; j < chunk_lenght; j++) {
    //             printf("%d ", *p);
    //             // Move the pointer to the next element
    //             p++;
    //         }
    //     }
    //     printf("\n");
    //     k++;
    //     // Move the pointer to the next row
    //     chunked[i]++;
    // }

    // It returns a pointer to the array of "rows/arrays"
    return *chunked;
}

// It fills the array with 0's if its length is smaller than the input lenght until it's equal to the input lenght
int *fillZeros(int *bits, int bitsLenght, int desiredLenght, char *endian) {
    // int l = *(&bits + 1) - bits;
    int zerosToAdd = desiredLenght - bitsLenght;
    // printf("%lu\n", *(&bits + 1) - bits);
    int *bitsOutput = malloc(sizeof(int) * desiredLenght);
    // If endian is equal to "LE" (little-endian) it will insert the zeros in the end of the array
    if (strcmp(endian, "LE") == 0) {
        for (int i = 0; i < desiredLenght; i++) {
            if (i >= bitsLenght) {
                *(bitsOutput + i) = 0;
            } else {
                *(bitsOutput + i) = *(bits + i);
            }
        }
    // Otherwise "BE" (big-endian) it will insert the zeros in the beginning of the array
    } else {
        int counter = 0;
        for (int i = 0; i < desiredLenght; i++) {
            if (i < zerosToAdd) {
                *(bitsOutput + i) = 0;
            } else {
                *(bitsOutput + i) = *(bits + counter);
                counter++;
            }
        }
    }

    return bits;
}