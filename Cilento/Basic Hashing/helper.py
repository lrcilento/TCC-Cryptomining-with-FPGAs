import math

# Helper function to append zeros to a bitstream
def fillZeros(bitstream, length, start):
    
    if start:
        while len(bitstream) < length:
            bitstream.insert(0, 0)
    
    else:
        while len(bitstream) < length:
            bitstream.append(0)

    return bitstream

# Helper function to divide a bitstream into chunks
def chunker(bitstream, chunk_size):

    chunked_bitstream = []

    # First we check if it is even possible to divide our bitstream equally in the chunk size provided
    if len(bitstream) % chunk_size != 0:
        return "Invalid chunk size, it has to divide the bitstream length"
    
    else:
        chunk_amount = int(len(bitstream) / chunk_size)

    # Then we sweep our array dividing it into our chunks
    for multiplier in range(0, chunk_amount):
        chunk = []
        offset = chunk_size * multiplier

        for bit in range(offset, offset + chunk_size):
            chunk.append(bitstream[bit])

        chunked_bitstream.append(chunk)

    return chunked_bitstream

# Formats and prepares our constants for processing
def initializer(initial_hash_values, is_the_input_a_list):

    binaries = []
    bitstream = []

    # Here we convert our values first to decimal from hexadecimal
    # Then we convert it to binary and cut it's "b" indicator
    for value in initial_hash_values:

        # Workaround to accepts list and strings
        if not is_the_input_a_list:
            value = initial_hash_values

        value = value.split('x')
        value = int(value[1], 16)
        value = bin(value)
        value = value.split('b')
        binaries.append(value[1])

        if not is_the_input_a_list:
            break

    # Now we need to convert it to a binary list from it's string representation
    for character in binaries:
        for bit in character:
            bitstream.append(int(bit))

        # Here we use our helper funciton to insert zeros at the start of the bitstream until it has a lenght of 32
        bitstream = fillZeros(bitstream, 32, True)

    return bitstream

# Convert our string into a bitstream
def toBitstream(message):

    hexadecimal_characters = []
    binary_characters = []
    bitstream = []

    # This will convert every character in our string to its hexadecimal form and insert it into a vector
    for character in message:
        hexadecimal_characters.append(ord(character))

    # This transforms our hexadecimal vector in a binary one
    # We remove the python binary indicator "0b" by spliting it and getting everything after "b"
    # Then prepending the "0" we lost in the process
    for character in hexadecimal_characters:
        binary_characters.append(bin(character).split("b")[1].zfill(8))

    # We now need to create a one dimensional list of all bits
    for character in binary_characters:
        for bit in character:
            bitstream.append(int(bit))

    return bitstream

# To prepare our message we will need to follow three main steps
# 1. Append a single '1' to the end of the message
# 2. Determine the bitstream length
# 3. Chunk it in a way that the total bitstream length is a multiple of 512
def preprocess(message):

    bitstream = toBitstream(message)

    # We need to get the length of our bitstream in a bitstream with lenght of 64
    length_bitstream = bin(len(bitstream)).split('b')[1]
    length_bitstream = [int(b) for b in length_bitstream.zfill(64)]

    # If our bitstream length is smaller than 448 we will handle it as an individual block
    # 448 because we will add a 64 bitstream to it to represent the initial message length, adding to 512
    if len(bitstream) < 448:
        # Step 1.
        bitstream.append(1)
        # Fill it with zeros until it reaches the length of 448
        bitstream = fillZeros(bitstream, 448, False)
        # Add the length bistream to it, totalizing 512 characters
        bitstream = bitstream + length_bitstream
        # Return it
        return [bitstream]

    # And if longer than 448 create multiple of 512 - 64 bits for the length at the end of the message
    else:
        # Step 1.
        bitstream.append(1)
        # Loop until total bitstream length is divisible by 512
        while (len(bitstream)+64) % 512 != 0:
            bits.append(0)
        # Add length bistream to it   
        bitstream = bitstream + length_bitstream
        # Return it in 512 bit chunks
        return chunker(bitstream, 512)