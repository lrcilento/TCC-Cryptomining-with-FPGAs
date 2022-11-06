from utils.helpers import b2Tob16, preprocessMessage, chunker, initializer
from utils.utils import *
from utils.constants import *

# SHA-256 main function
def sha256(message):
    # It initializes the values of "K" (array of hexadecimal constants) by converting them into arrays of bits
    k = initializer(K)
    # It initializes the values of "H_HEX" (array of hexadecimal constants) by converting them into arrays of bits
    # Attributes each array returned in the list in a different variable
    h0, h1, h2, h3, h4, h5, h6, h7 = initializer(H_HEX)
    # Returns an array of 512 bits or a list of arrays of 512 bits
    chunks = preprocessMessage(message)
    # Iterates the list, passing by every array inside the list
    for chunk in chunks:
        # Splits the input array of bits in arrays of 32 bits, and returns them inside a list of arrays
        w = chunker(chunk, 32)
        # Appends 48 arrays of 32 bits of 0's in the end of the "w" list
        for _ in range(48):
            w.append(32 * [0])
        # Iterates the "w" list starting at the first array of 0 chunks added a step before
        # Logic to mix everything to turn the input in a hash
        for i in range(16, 64):
            s0 = XORXOR(rotr(w[i - 15], 7), rotr(w[i - 15], 18), shr(w[i - 15], 3))
            s1 = XORXOR(rotr(w[i - 2], 17), rotr(w[i - 2], 19), shr(w[i - 2], 10))
            w[i] = add(add(add(w[i - 16], s0), w[i - 7]), s1)
        a = h0
        b = h1
        c = h2
        d = h3
        e = h4
        f = h5
        g = h6
        h = h7
        # More mixing
        for j in range(64):
            S1 = XORXOR(rotr(e, 6), rotr(e, 11), rotr(e, 25))
            ch = XOR(AND(e, f), AND(NOT(e), g))
            temp1 = add(add(add(add(h, S1), ch), k[j]), w[j])
            S0 = XORXOR(rotr(a, 2), rotr(a, 13), rotr(a, 22))
            m = XORXOR(AND(a, b), AND(a, c), AND(b, c))
            temp2 = add(S0, m)
            h = g
            g = f
            f = e
            e = add(d, temp1)
            d = c
            c = b
            b = a
            a = add(temp1, temp2)
        h0 = add(h0, a)
        h1 = add(h1, b)
        h2 = add(h2, c)
        h3 = add(h3, d)
        h4 = add(h4, e)
        h5 = add(h5, f)
        h6 = add(h6, g)
        h7 = add(h7, h)
    digest = ''
    for val in [h0, h1, h2, h3, h4, h5, h6, h7]:
        digest += b2Tob16(val) + " "
    return digest


# The start point
if __name__ == '__main__':
    # Gets the user input
    input_message = input('Type a text: ')
    # Calls the sha256 function
    # Prints the final hash output
    print('Hash:', sha256(input_message))