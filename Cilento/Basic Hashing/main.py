from helper import *
from utils import *
from constants import *

message = 'abc'

# Initialize values and constants for future use
# Constants are not coming back as a singular bitstream, I don't know if that's correct <======== CAUTION
for index in range(0, len(constants)):
    constants[index] = initializer(constants[index], False)
for index in range(0, len(initial_hash_values)):
    initial_hash_values[index] = initializer(initial_hash_values[index], False)

# Transcribe our string into bitstreams of 512 bits
chunks = preprocess(message)
for chunk in chunks:
    # Create smaller bitstreams of 32 bits (512 bits / 32 = 16 smalles chunks)
    smaller_chunks = chunker(chunk, 32)
    # Now we create 48 'empty' bistreams with 32 bits
    # Totalizing 64 smaller chunks, 16 containing our message and 48 empty ones
    for i in range(0, 48):
        aux_bitstream = []
        for bit in range(0, 32):
            aux_bitstream.append(0)
        smaller_chunks.append(aux_bitstream)

    # Starting from smaller_chunks[16], which is the first zero-only list, we will iterate over the message
    for i in range(16, 64):
        # For every small_chunk between indexes 1 and 49 we will double XOR the results of rotating it right by 7, 18 and 3 spaces
        aux_0 = XORXOR(rotateRight(smaller_chunks[i-15], 7), rotateRight(smaller_chunks[i-15], 18), shiftRight(smaller_chunks[i-15], 3))
        # Here we do something similiar but using small_chunks between 14 and 62, and also changing the rotating amount to 17, 19 and 10
        aux_1 = XORXOR(rotateRight(smaller_chunks[i-2], 17), rotateRight(smaller_chunks[i-2], 19), shiftRight(smaller_chunks[i-2], 10))
        # Now small_chunk[i] becames the sum of small_chunk[i-16], aux_0, small_chunk[i-17] and aux1
        smaller_chunks[i] = add(add(add(smaller_chunks[i-16], aux_0), smaller_chunks[i-7]), aux_1)
        # The goal here is to shuffle the bits in a deterministic way
        # Right now we have our message still intact in binary form within the first 16 segments of our list
        # And as of right now, a shuffled seeded mess from there onward
    # Now we need to initialize our intermediary variables with the initial hash values h0 ~ h7
    intermediary_1 = initial_hash_values[0]
    intermediary_2 = initial_hash_values[1]
    intermediary_3 = initial_hash_values[2]
    intermediary_4 = initial_hash_values[3]
    intermediary_5 = initial_hash_values[4]
    intermediary_6 = initial_hash_values[5]
    intermediary_7 = initial_hash_values[6]
    intermediary_8 = initial_hash_values[7]
    # Now we'll start the compression part
    for j in range(64):
        # Same idea as above, but now working with our intermediary values
        aux_2 = XORXOR(rotateRight(intermediary_5, 6), rotateRight(intermediary_5, 11), rotateRight(intermediary_5, 25))
        choose = XOR(AND(intermediary_5, intermediary_6), AND(NOT(intermediary_5), intermediary_7))
        aux_3 = add(add(add(add(intermediary_8, aux_2), choose), constants[j]), smaller_chunks[j])
        aux_4 = XORXOR(rotateRight(intermediary_1, 2), rotateRight(intermediary_1, 13), rotateRight(intermediary_1, 22))
        aux_5 = XORXOR(AND(intermediary_1, intermediary_2), AND(intermediary_1, intermediary_3), AND(intermediary_2, intermediary_3))
        aux_6 = add(aux_4, aux_5)
        # We're now over with the compression and we need to shuffle our intermediary values around, otherwise every bit would get the same result
        intermediary_8 = intermediary_7
        intermediary_7 = intermediary_6
        intermediary_6 = intermediary_5
        intermediary_5 = add(intermediary_4, aux_3)
        intermediary_4 = intermediary_3
        intermediary_3 = intermediary_2
        intermediary_2 = intermediary_1
        intermediary_1 = add(aux_3, aux_6)
        # We could basically use a shift register chain to pass around most of them
        # Aside from intermerdiary 1 and 5 which depends on our auxiliary values, as they are used in the compression
    # Following the same login, we now need to shuffle our initial hash values so our different chunks generate different results
    initial_hash_values[0] = add(initial_hash_values[0], intermediary_1)
    initial_hash_values[1] = add(initial_hash_values[1], intermediary_2)
    initial_hash_values[2] = add(initial_hash_values[2], intermediary_3)
    initial_hash_values[3] = add(initial_hash_values[3], intermediary_4)
    initial_hash_values[4] = add(initial_hash_values[4], intermediary_5)
    initial_hash_values[5] = add(initial_hash_values[5], intermediary_6)
    initial_hash_values[6] = add(initial_hash_values[6], intermediary_7)
    initial_hash_values[7] = add(initial_hash_values[7], intermediary_8)

# For simplicity's sake we will transform our bitstream back to a string format
for index in range(0, len(initial_hash_values)):
    binary = ''
    for bit in initial_hash_values[index]:
        binary = binary + str(bit)
    initial_hash_values[index] = binary

# Finally we present it in it's hexadecimal form to facilitate testing
digest = ''
for val in initial_hash_values:
    digest += str(hex(int(val, 2))).split('x')[1] + ' '

print(digest)
a = rotateRight([0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0], 17)
b = rotateRight([0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0], 19)
c = shiftRight([0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0], 10)
print(XORXOR(a, b , c))
