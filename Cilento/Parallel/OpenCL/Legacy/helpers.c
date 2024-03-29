#include "helpers.h"

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
    for (int i = 0; i < strlen(message); i++) {
        char *tempCode = bin(*(charCodes + i));
        char *code = malloc(8);
        
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
    int *bits = malloc((strlen(message) * 8) * sizeof(int));
    for (int i = 0; i < sizeof(bytes) / 8; i++) {
        for (int j = 0; j < strlen(bytes[i]); j++) {
            bits[counter] = bytes[i][j];
            counter++;
        }
    }
    // It returns a single list containing all the bits of the input characters
    return bits;
}

// // It splits the input array of bits in arrays of 8 bits, and returns them inside a list of arrays
// int *chunker(int *bits, int bitsLenght, int chunkLenght) {
//     int numRows = bitsLenght / chunkLenght;
//     int chunked[numRows][chunkLenght];
//     int counter = 0;
//     for (int i = 0; i < numRows; i++) {
//         for (int j = 0; j < chunkLenght; j++) {
//             chunked[i][j] = bits[counter];
//             counter++;
//         }
//     }

//     // for (int i = 0; i < numRows; i++) {
//     //     for (int j = 0; j < chunkLenght; j++) {
//     //         printf("%c", chunked[i][j]);
//     //     }
//     // }
//     // printf("\n");

//     return chunked;
// }

// It fills the array with 0's if its length is smaller than the input lenght until it's equal to the input lenght
// The zeros (48) and ones (49) are in ASCII
int *fillZeros(int *bits, int bitsLenght, int desiredLenght, char *endian) {
    if (bitsLenght >= desiredLenght) {
        return bits;
    }
    int zerosToAdd = desiredLenght - bitsLenght;
    int *bitsOutput = malloc(desiredLenght * sizeof(int));
    // If endian is equal to "LE" (little-endian) it will insert the zeros in the end of the array
    if (strcmp(endian, "LE") == 0) {
        for (int i = 0; i < desiredLenght; i++) {
            if (i >= bitsLenght) {
                *(bitsOutput + i) = 48;
            } else {
                *(bitsOutput + i) = *(bits + i);
            }
        }
    // Otherwise "BE" (big-endian) it will insert the zeros in the beginning of the array
    } else {
        int counter = 0;
        for (int i = 0; i < desiredLenght; i++) {
            if (i < zerosToAdd) {
                *(bitsOutput + i) = 48;
            } else {
                *(bitsOutput + i) = *(bits + counter);
                counter++;
            }
        }
    }

    return bitsOutput;
}

// It returns an array of 512 bits
int *preprocessMessage(char *message) {
    // Translates the message in an array of bits, (8 bits for every character in the message)
    int *bits = translate(message);
    // # Takes the length of the array of bits (multiple of eight)
    int lenght = strlen(message) * 8;
    // Translates the length of the array to binary
    char *binLenght = bin(lenght);
    // Chops off the 0b binary indicator (e.g. 0b11000 -> 11000)
    // Fills the beggining of the string with 0's until it has a length of 64 characters (64 bits)
    // Converts each bit of the string to an integer
    // Appends each of the converted bits to an array
    char *choppedBinLenght = malloc(strlen(binLenght) - 2);
    int counter = 0;
    for (int i = 0; i < strlen(binLenght); i++) {
        if (i >= 2) {
            choppedBinLenght[counter] = *(binLenght + i);
            counter++;
        }
    }
    counter = 0;
    char *filledBinLenght = zfill(choppedBinLenght, 64);
    free(choppedBinLenght);
    int messageLen[64];
    for (int i = 0; i < 64; i++) {
        messageLen[i] = (int) *(filledBinLenght + i);
    }
    // Appends a single 1 in the end of the array of bits of the translated input
    int *bitsIncreased = malloc((lenght + 1) * sizeof(int));
    for (int i = 0; i < lenght; i++) {
        *(bitsIncreased + i) = *(bits + i);
    }
    *(bitsIncreased + lenght) = 49;
    // Fills the end of the array with 0's until its length is equal to 448
    bits = fillZeros(bitsIncreased, lenght + 1, 448, "LE");
    free(bitsIncreased);
    // Sums the "bits" variable with the "messageLen" variable
    int *bitsMessageLenMerge = malloc(512 * sizeof(int));
    for (int i = 0; i < 512; i++) {
        if (i < 448) {
            *(bitsMessageLenMerge + i) = *(bits + i);
        } else {
            *(bitsMessageLenMerge + i) = (int) *(messageLen + counter);
            counter++;
        }
    }
    // Returns the bits variable inside a list
    return bitsMessageLenMerge;
}

// It initializes the values (array of hexadecimal constants) by converting them into arrays of bits
int *initializer(char *value) {
    // Chops off the 0b binary indicator
    char choppedCharValue[9];
    strcpy(choppedCharValue, value + 2);
    // Converts the value from hexadecimal to binary
    int alert = 0;
    char charBinValue[33];
    for (int i = 0; i < 8; i++) {
        switch (*(choppedCharValue + i)) {
            case '0':
                if (i == 0) {
                    strcpy(charBinValue, "");
                    alert = 1;
                    break;
                }
                strcat(charBinValue, "0000");
                break;
            case '1':
                if (i == 0) {
                    strcpy(charBinValue, "");
                }
                if (i == 0 || alert == 1) {
                    strcat(charBinValue, "1");
                    alert = 0;
                    break;
                }
                strcat(charBinValue, "0001");
                break;
            case '2':
                if (i == 0) {
                    strcpy(charBinValue, "");
                }
                if (i == 0 || alert == 1) {
                    strcat(charBinValue, "10");
                    alert = 0;
                    break;
                }
                strcat(charBinValue, "0010");
                break;
            case '3':
                if (i == 0) {
                    strcpy(charBinValue, "");
                }
                if (i == 0 || alert == 1) {
                    strcat(charBinValue, "11");
                    alert = 0;
                    break;
                }
                strcat(charBinValue, "0011");
                break;
            case '4':
                if (i == 0) {
                    strcpy(charBinValue, "");
                }
                if (i == 0 || alert == 1) {
                    strcat(charBinValue, "100");
                    alert = 0;
                    break;
                }
                strcat(charBinValue, "0100");
                break;
            case '5':
                if (i == 0) {
                    strcpy(charBinValue, "");
                }
                if (i == 0 || alert == 1) {
                    strcat(charBinValue, "101");
                    alert = 0;
                    break;
                }
                strcat(charBinValue, "0101");
                break;
            case '6':
                if (i == 0) {
                    strcpy(charBinValue, "");
                }
                if (i == 0 || alert == 1) {
                    strcat(charBinValue, "110");
                    alert = 0;
                    break;
                }
                strcat(charBinValue, "0110");
                break;
            case '7':
                if (i == 0) {
                    strcpy(charBinValue, "");
                }
                if (i == 0 || alert == 1) {
                    strcat(charBinValue, "111");
                    alert = 0;
                    break;
                }
                strcat(charBinValue, "0111");
                break;
            case '8':
                if (i == 0) {
                    strcpy(charBinValue, "");
                }
                if (alert == 1) {
                    alert = 0;
                }
                strcat(charBinValue, "1000");
                break;
            case '9':
                if (i == 0) {
                    strcpy(charBinValue, "");
                }
                if (alert == 1) {
                    alert = 0;
                }
                strcat(charBinValue, "1001");
                break;
            case 'A':
            case 'a':
                if (i == 0) {
                    strcpy(charBinValue, "");
                }
                if (alert == 1) {
                    alert = 0;
                }
                strcat(charBinValue, "1010");
                break;
            case 'B':
            case 'b':
                if (i == 0) {
                    strcpy(charBinValue, "");
                }
                if (alert == 1) {
                    alert = 0;
                }
                strcat(charBinValue, "1011");
                break;
            case 'C':
            case 'c':
                if (i == 0) {
                    strcpy(charBinValue, "");
                }
                if (alert == 1) {
                    alert = 0;
                }
                strcat(charBinValue, "1100");
                break;
            case 'D':
            case 'd':
                if (i == 0) {
                    strcpy(charBinValue, "");
                }
                if (alert == 1) {
                    alert = 0;
                }
                strcat(charBinValue, "1101");
                break;
            case 'E':
            case 'e':
                if (i == 0) {
                    strcpy(charBinValue, "");
                }
                if (alert == 1) {
                    alert = 0;
                }
                strcat(charBinValue, "1110");
                break;
            case 'F':
            case 'f':
                if (i == 0) {
                    strcpy(charBinValue, "");
                }
                if (alert == 1) {
                    alert = 0;
                }
                strcat(charBinValue, "1111");
                break;
        }
    }
    // Converts the char* to int
    int *word = malloc(strlen(charBinValue) * sizeof(int));
    for (int i = 0; i < strlen(charBinValue); i++) {
        word[i] = *(charBinValue + i);
    }
    // Fills the beggining of the array with 0's until it has the lenght of 32 bits
    int *wordFilled = fillZeros(word, strlen(charBinValue), 32, "BE");
    return wordFilled;
}