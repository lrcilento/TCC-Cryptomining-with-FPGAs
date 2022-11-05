# <Python built-in functions rewrited (overwrited)>

# It receives an integer input and returns a binary string
def bin(input):
    # If the input is == 0, it returns a hardcoded string
    if input == 0:
        return '0b0'
    # It creates an empty binary string
    binaryString = ''
    # It does a while loop that contains the convertion logic, to create a binary number
    # But the logic creates the final binary output inverted
    while(input > 0):
        digit = input % 2
        binaryString += str(digit)
        input = input // 2
    # It creates an empty binary string, that will contain the function binary output
    invertedBinaryString = ''
    # It does the logic to invert the binary string
    counter = len(binaryString) - 1
    while counter >= 0:
        invertedBinaryString += binaryString[counter]
        counter -= 1
    # It concatenates the 0b binary indicator to the beggining of the binary string output
    return '0b' + invertedBinaryString


# Implementing the entire ASCII table? D:
# def ord(input):
#     pass


# It fills the beggining of the string input with 0's until it has the same length as the integer input
def zfill(strInput, intInput):
    # It will let to be True only when triggering the break
    while True:
        # If the strInput length is less than the intInput
        if len(strInput) < intInput:
            # Concatenates a 0 in the beggining of the string
            strInput = '0' + strInput
        # Otherwise
        else:
            # Breaks the loop
            break
    # Returns the function output
    return strInput

# </Python built-in functions rewrited (overwrited)>

# It returns the string input as an array of bits output (8 bits for every character in the input)
def translate(message):
    # Translates the input to the ASCII equivalent
    charcodes = [ord(c) for c in message]
    # Translates the ASCII integer to binary
    # Chops off the 0b binary indicator (e.g. 0b1001000 -> 1001000)
    # Fills the beggining of the string with 0's until it has a length of 8 characters (8 bits)
    bytes = []
    for char in charcodes:
        bytes.append(zfill(bin(char)[2:], 8))
    # Converts the string of bits in an array of bits
    bits = []
    for byte in bytes:
        for bit in byte:
            bits.append(int(bit))
    return bits


# It splits the input array of bits in arrays of 8 bits, and returns them inside a list of arrays
def chunker(bits, chunk_length = 8):
    chunked = []
    print(len(bits))
    for b in range(0, len(bits), chunk_length):
        chunked.append(bits[b:b + chunk_length])
    return chunked


# It fills the array with 0's if its length is smaller than 8 until it's equal to 8
def fillZeros(bits, length = 8, endian = 'LE'):
    l = len(bits)
    # If endian is equal to 'LE' (little-endian) it will insert the zeros in the end of the array
    if endian == 'LE':
        for i in range(l, length):
            bits.append(0)
    # Otherwise 'BE' (big-endian) it will insert the zeros in the beginning of the array
    else:
        while l < length:
            bits.insert(0, 0)
            l = len(bits)
    return bits


# It returns an array of 512 bits or it sends to the "chunker" function to return a list of arrays of 512 bits
def preprocessMessage(message):
    # Translates the message in an array of bits, (8 bits for every character in the message)
    bits = translate(message)
    # Takes the length of the array of bits (multiple of eight)
    length = len(bits)
    # Translates the length of the array to binary
    # Chops off the 0b binary indicator (e.g. 0b11000 -> 11000)
    # Fills the beggining of the string with 0's until it has a length of 64 characters (64 bits)
    # Converts each bit of the string to an integer
    # Appends each of the converted bits to an array
    message_len = [int(b) for b in zfill(bin(length)[2:], 64)]
    # If the length of bits of the translated input is less than 448
    if length < 448:
        # Appends a single 1 in the end of the array of bits of the translated input
        bits.append(1)
        # Fills the end of the array with 0's until its length is equal to 448
        bits = fillZeros(bits, 448, 'LE')
        # Sums the "bits" variable with the "message_len" variable
        bits = bits + message_len
        # Returns the bits variable inside a list
        return [bits]
    # Otherwise if the length of the bits of the translated input is equal to 448
    elif length == 448:
        # Appends a single 1 in the end of the array of bits of the translated input
        bits.append(1)
        # Fills the end of the array with 0's until its length is equal to 1024
        bits = fillZeros(bits, 1024, 'LE')
        # Takes all the 64 bits of the "message_len" array
        # Puts those bits in the place of the last 64 bits of the "bits" array
        bits[-64:] = message_len
        # Sends the result to the "chunker" function
        return chunker(bits, 512)
    # Otherwise
    else:
        # Appends a single 1 in the end of the array of bits of the translated input
        bits.append(1)
        # Increases the size of the "bits" array while it's not divisible by 512
        while len(bits) % 512 != 0:
            bits.append(0)
        # Takes all the 64 bits of the "message_len" array
        # Puts those b  its in the place of the last 64 bits of the "bits" array
        bits[-64:] = message_len
    # Sends the result to the "chunker" function
    return chunker(bits, 512)


# It initializes the values (array of hexadecimal constants) by converting them into arrays of bits
def initializer(values):
    # Converts the values from hexadecimal to binary and chops off the 0b binary indicator
    binaries = [bin(int(v, 16))[2:] for v in values]
    # Splits every bit in every position of the array
    # Turns every position in a new array of bits
    # Inserts all the arrays inside a list, making a list of arrays
    words = []
    for binary in binaries:
        word = []
        for b in binary:
            word.append(int(b))
        # Fills every array of bits that's not multiple of 8 with 0's in the beginning, until it turns multiple of 8
        words.append(fillZeros(word, 32, 'BE'))
    return words


# It returns the Base 2 array input as a string Base 16 output
def b2Tob16(value):
    # Converts an array of bits in a string of bits
    value = ''.join([str(x) for x in value])
    # Splits the string of bits in chuncks of 4 bits
    # Appends each of the chuncks in an array position
    # Adds the 0b binary indicator in the beggining of all the array positions
    binaries = []
    for d in range(0, len(value), 4):
        binaries.append('0b' + value[d:d + 4])
    # Converts each of the binary 4 bits chuncks in the array to hexadecimal
    # Chops off the 0x binary indicator (e.g. 0x48 -> 48)
    # Converts each of the hexadecimal chuncks to string
    # Concatenates the hexadecimals chuncks into a single string
    hexes = ''
    for b in binaries:
        hexes += hex(int(b, 2))[2:]
    return hexes