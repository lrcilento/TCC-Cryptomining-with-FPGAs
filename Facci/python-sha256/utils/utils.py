# False == 0 | True != False (0)

# Is True
# Returns True if input "x" is equal to 1
def isTrue(x):
    return x == 1

# Simple If
# If "i" is equal to 1 (True) it returns "y", else (False) it returns "z"
def if_(i, y, z): 
    return y if isTrue(i) else z

# <LOGIC GATES>

# AND
# If both the inputs == True, the output will be True
# Otherwise, the output will be False
def and_(i, j): return if_(i, j, 0)
# It does the same for lists
def AND(i, j): return [and_(ia, ja) for ia, ja in zip(i, j)]

# NOT
# It inverts (negates) the input
def not_(i):
    return if_(i, 0, 1)
# It does the same for lists
def NOT(i): return [not_(x) for x in i]

# XOR (Exclusive OR)
# If there's only one input == True, the output will be True
# Otherwise, if there's no input == True, or if there's more than one input == True, the output will be False
def xor_(i, j):
    return if_(i, not_(j), j)
# It does the same for lists
def XOR(i, j): return [xor_(ia, ja) for ia, ja in zip(i, j)]

# XORXOR
# The output will be True if the number of values == True is odd
def xorxor(i, j, l):
    return xor_(i, xor_(j, l))
# It does the same for lists
def XORXOR(i, j, l): return [xorxor(ia, ja, la) for ia, ja, la, in zip(i, j, l)]

# </LOGIC GATES>

# Majority
# It returns the input with the majority of results
def maj(i, j, k):
    if i == j or i == k:
        return i
    else:
        return j

# Rotate Right
# It takes the bits that are before the break point and puts them at the end of the array
def rotr(x, n):
    y = []
    breakPoint = len(x) - n
    counter = 0
    while counter < len(x):
        if counter < breakPoint:
            pass
        else:
            y.append(x[counter])
        counter += 1
    for i in range(breakPoint):
        y.append(x[i])
    return y

# Shift Right
# It takes a number of bits to shift right filling with 0's
def shr(x, n):
    y = []
    for i in range(n):
        y.append(0)
    for i in x:
        y.append(i)
    return y

# Add
# It takes two lists of bits and adds them
def add(i, j):
    length = len(i)
    sums = list(range(length))
    c = 0
    for x in range(length-1,-1,-1):
        sums[x] = xorxor(i[x], j[x], c)
        c = maj(i[x], j[x], c)
    return sums