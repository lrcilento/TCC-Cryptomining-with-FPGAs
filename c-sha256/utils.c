#include "utils.h"

// False == 0 | True != False (0)

// It returns a list of tuples, joining the same index of the two input arrays together in a tuple
// Each tuple contains two values of the input arrays, both from the same index from the two different arrays
tuple *zip(int x[], int y[]) {
    static tuple arrayOfTuples[32];

    for (int i = 0; i < 32; i++) {
        arrayOfTuples[i].number0 = x[i];
        arrayOfTuples[i].number1 = y[i];
    }

    return arrayOfTuples;
}
// It zips three instead of two
thraay *zipThree(int x[], int y[], int z[]) {
    static thraay arrayOfThraays[32];

    for (int i = 0; i < 32; i++) {
        arrayOfThraays[i].number0 = x[i];
        arrayOfThraays[i].number1 = y[i];
        arrayOfThraays[i].number2 = z[i];
    }

    return arrayOfThraays;
}

// Is True
// Returns True if input "x" is equal to 1
bool isTrue(int x) {
    // printf("%c", x);
    // printf("\n");
    if (x == 1) {
        return true;
    } else {
        return false;
    }
}

// Simple If
// If "i" is equal to 1 (True) it returns "y", else (False) it returns "z"
int if_(int i, int y, int z) {
    printf("%c %c %c", i, y, z);
    printf("\n");
    if (isTrue(i)) {
        return y;
    } else {
        return z;
    }
}

// <LOGIC GATES>

// AND
// If both the inputs == True, the output will be True
// Otherwise, the output will be False
int and_(int i, int j) {
    return if_(i, j, 0);
}
// It does the same for lists
int *AND(int x[], int y[]) {
    tuple *arrayOfTuples = zip(x, y);
    static int andList[32];

    for (int i = 0; i < 32; i++) {
        andList[i] = and_(arrayOfTuples[i].number0, arrayOfTuples[i].number1);
    }

    return andList;
}

// NOT
// It inverts (negates) the input
int not_(int i) {
    // printf("%c", i);
    // printf("\n");
    return if_(i, 0, 1);
}
// It does the same for lists
int *NOT(int x[]) {
    static int notList[32];

    for (int i = 0; i < 32; i++) {
        notList[i] = not_(x[i]);
    }

    return notList;
}

// XOR (Exclusive OR)
// If there's only one input == True, the output will be True
// Otherwise, if there's no input == True, or if there's more than one input == True, the output will be False
int xor_(int i, int j) {
    // printf("%c %c", i, j);
    // printf("\n");
    return if_(i, not_(j), j);
}
// It does the same for lists
int *XOR(int x[], int y[]) {
    tuple *arrayOfTuples = zip(x, y);
    static int xorList[32];

    for (int i = 0; i < 32; i++) {
        xorList[i] = xor_(arrayOfTuples[i].number0, arrayOfTuples[i].number1);
    }

    return xorList;
}

// XORXOR
// The output will be True if the number of values == True is odd
int xorxor(int i, int j, int l) {
    // printf("%c %c %c", i, j, l);
    // printf("\n");
    return xor_(i, xor_(j, l));
}
// It does the same for lists
int *XORXOR(int x[], int y[], int z[]) {
    thraay *arrayOfThraays = zipThree(x, y, z);
    static int xorXorList[32];

    for (int i = 0; i < 32; i++) {
        xorXorList[i] = xorxor(arrayOfThraays[i].number0, arrayOfThraays[i].number1, arrayOfThraays[i].number2);
    }

    return xorXorList;
}

// </LOGIC GATES>

// Majority
// It returns the input with the majority of results
int maj(int i, int j, int k) {
    if (i == j || i == k) {
        return i;
    } else {
        return j;
    }
}

// Rotate Right
// It takes the bits that are before the break point and puts them at the end of the array
int *rotr(int x[], int size, int n) {
    int *y = malloc(size * sizeof(int));
    int breakPoint = size - n;
    int indexCounter = 0;
    int counter = 0;
    
    while (counter < size) {
        if (counter >= breakPoint) {
            y[indexCounter] = x[counter];
            indexCounter++;
        }
        counter++;
    }

    for (int i = 0; i < breakPoint; i++) {
        y[indexCounter] = x[i];
        indexCounter++;
    }

    return y;
}

// Shift Right
// It takes a number of bits to shift right filling with 0's
int *shr(int x[], int size, int n) {
    int *y = malloc((size + n) * sizeof(int));
    int indexCounter = 0;

    for (int i = 0; i < n; i++) {
        y[indexCounter] = 48;
        indexCounter++;
    }

    for (int i = 0; i < size; i++) {
        y[indexCounter] = x[i];
        indexCounter++;
    }

    return y;
}

// Add
// It takes two lists of bits and adds them
int *add(int x[], int y[], int size) {
    int *sums = malloc(size * sizeof(int));
    int c = 0;

    for (int i = size - 1; i > 0; i--) {
        sums[i] = xorxor(x[i], y[i], c);
        c = maj(x[i], y[i], c);
    }

    return sums;
}