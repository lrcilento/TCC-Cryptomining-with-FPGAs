// Function prototypes
bool init();

using namespace aocl_utils;

// ACL runtime configuration
static cl_platform_id platform = NULL;
static cl_device_id device = NULL;
static cl_context context = NULL;
static cl_command_queue queue = NULL;
static cl_kernel kernel = NULL;
static cl_program program = NULL;
static cl_int status = 0;

// Control whether the emulator should be used.
static bool use_emulator = false;

char *bin(int input);
char *zfill(char *strInput, int intInput);
int *translate(char *message);
int *chunker(int *bits, int bitsLenght, int chunkLenght);
int *fillZeros(int *bits, int bitsLenght, int desiredLenght, char *endian);
int *preprocessMessage(char *message);
int *initializer(char *value);
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

bool init() {
	cl_int status;

	if(!setCwdToExeDir()) {
		return false;
	}

	// Get the OpenCL platform.
	platform = findPlatform("Intel(R) FPGA SDK for OpenCL(TM)");
	
	if(platform == NULL) {
		printf("ERROR: Unable to find Intel(R) FPGA OpenCL platform\n");
		return false;
	}

	// Query the available OpenCL devices.
	scoped_array<cl_device_id> devices;
	cl_uint num_devices;

	devices.reset(getDevices(platform, CL_DEVICE_TYPE_ALL, &num_devices));

	// We'll just use the first device.
	device = devices[0];

	// Create the context.
	context = clCreateContext(NULL, 1, &device, &oclContextCallback, NULL, &status);
	checkError(status, "Failed to create context");

	// Create the command queue.
	queue = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &status);
	checkError(status, "Failed to create command queue");

	// Create the program.
	std::string binary_file = getBoardBinaryFile("stmi", device);
	printf("Using AOCX: %s\n\n", binary_file.c_str());
	program = createProgramFromBinary(context, binary_file.c_str(), &device, 1);

	// Build the program that was just created.
	status = clBuildProgram(program, 0, NULL, "", NULL, NULL);
	checkError(status, "Failed to build program");

	// Create the kernel - name passed in here must match kernel name in the
	// original CL file, that was compiled into an AOCX file using the AOC tool
	const char *kernel_name = "stmi";  // Kernel name, as defined in the CL file
	kernel = clCreateKernel(program, kernel_name, &status);
	checkError(status, "Failed to create kernel");

  	return true;
}

tuple *zip(int x[], int y[]) {
    static tuple arrayOfTuples[32];

    for (int i = 0; i < 32; i++) {
        arrayOfTuples[i].number0 = x[i];
        arrayOfTuples[i].number1 = y[i];
    }

    return arrayOfTuples;
}

thraay *zipThree(int x[], int y[], int z[]) {
    static thraay arrayOfThraays[32];

    for (int i = 0; i < 32; i++) {
        arrayOfThraays[i].number0 = x[i];
        arrayOfThraays[i].number1 = y[i];
        arrayOfThraays[i].number2 = z[i];
    }

    return arrayOfThraays;
}

int isTrue(int x) {
    if (x == 49) {
        return 48;
    } else {
        return 49;
    }
}

int if_(int i, int y, int z) {
    if (isTrue(i) == 48) {
        return y;
    } else {
        return z;
    }
}

int and_(int i, int j) {
    return if_(i, j, 48);
}

int *AND(int x[], int y[]) {
    tuple *arrayOfTuples = zip(x, y);
    int *andList = (int *) malloc(32 * sizeof(int));

    for (int i = 0; i < 32; i++) {
        andList[i] = and_(arrayOfTuples[i].number0, arrayOfTuples[i].number1);
    }

    return andList;
}

int not_(int i) {
    return if_(i, 48, 49);
}

int *NOT(int x[]) {
    int *notList = (int *) malloc(32 * sizeof(int));

    for (int i = 0; i < 32; i++) {
        notList[i] = not_(x[i]);
    }

    return notList;
}

int xor_(int i, int j) {
    return if_(i, not_(j), j);
}

int *XOR(int x[], int y[]) {
    tuple *arrayOfTuples = zip(x, y);
    int *xorList = (int *) malloc(32 * sizeof(int));

    for (int i = 0; i < 32; i++) {
        xorList[i] = xor_(arrayOfTuples[i].number0, arrayOfTuples[i].number1);
    }

    return xorList;
}

int xorxor(int i, int j, int l) {
    return xor_(i, xor_(j, l));
}

int *XORXOR(int x[], int y[], int z[]) {
    thraay *arrayOfThraays = zipThree(x, y, z);
    int *xorXorList = (int *) malloc(32 * sizeof(int));

    for (int i = 0; i < 32; i++) {
        xorXorList[i] = xorxor(arrayOfThraays[i].number0, arrayOfThraays[i].number1, arrayOfThraays[i].number2);
    }

    return xorXorList;
}

int maj(int i, int j, int k) {
    if (i == j || i == k) {
        return i;
    } else {
        return j;
    }
}

int *rotr(int x[], int size, int n) {
    int *y = (int *) malloc(size * sizeof(int));
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

int *shr(int x[], int size, int n) {
    int *y = (int *) malloc((size + n) * sizeof(int));
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

int *add(int x[], int y[], int size) {
    int *sums = (int *) malloc(size * sizeof(int));
    int c = 48;

    for (int i = size - 1; i >= 0; i--) {
        sums[i] = xorxor(x[i], y[i], c);
        c = maj(x[i], y[i], c);
    }

    return sums;
}

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
    char *binaryString = (char *) malloc(firstCounter);
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
    char *invertedBinaryString = (char *) malloc(firstCounter + 2);
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

char *zfill(char *strInput, int intInput) {
    char *strOutput = (char *) malloc(intInput);
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
        char *code = (char *) malloc(8);
        
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
    int *bits = (int *) malloc((strlen(message) * 8) * sizeof(int));
    for (int i = 0; i < sizeof(bytes) / 8; i++) {
        for (int j = 0; j < strlen(bytes[i]); j++) {
            bits[counter] = bytes[i][j];
            counter++;
        }
    }
    // It returns a single list containing all the bits of the input characters
    return bits;
}

int *fillZeros(int *bits, int bitsLenght, int desiredLenght, char *endian) {
    if (bitsLenght >= desiredLenght) {
        return bits;
    }
    int zerosToAdd = desiredLenght - bitsLenght;
    int *bitsOutput = (int *) malloc(desiredLenght * sizeof(int));
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
    char *choppedBinLenght = (char *) malloc(strlen(binLenght) - 2);
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
    int *bitsIncreased = (int *) malloc((lenght + 1) * sizeof(int));
    for (int i = 0; i < lenght; i++) {
        *(bitsIncreased + i) = *(bits + i);
    }
    *(bitsIncreased + lenght) = 49;
    // Fills the end of the array with 0's until its length is equal to 448
    bits = fillZeros(bitsIncreased, lenght + 1, 448, "LE");
    free(bitsIncreased);
    // Sums the "bits" variable with the "messageLen" variable
    int *bitsMessageLenMerge = (int *) malloc(512 * sizeof(int));
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
    int *word = (int *) malloc(strlen(charBinValue) * sizeof(int));
    for (int i = 0; i < strlen(charBinValue); i++) {
        word[i] = *(charBinValue + i);
    }
    // Fills the beggining of the array with 0's until it has the lenght of 32 bits
    int *wordFilled = fillZeros(word, strlen(charBinValue), 32, "BE");
    return wordFilled;
}