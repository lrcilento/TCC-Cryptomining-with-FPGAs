# Truth representation is integer 1
def isTrue(statement): 
    if statement == 1:
        return True
    else:
        return False

# Simple conditional
def if_(condition, positive_result, negative_result): 
    if isTrue(condition):
        return positive_result
    else:
        return negative_result

# AND Gate: Both x and y need to be true (1) to output a true statement
def and_(xValue, yValue):
    # Here it checks if x is true, if not the AND gate produces 0, repesented by the negative result
    # If it is true then the AND gate produces y, so to produce a 1 result both x and y have to be true (1)
    return if_(xValue, yValue, 0)

def AND(xList, yList):
    andResults = []
    # Here it checks the AND relation of every tuple within x and y cartesian product and outputs it to a list
    for xValue, yValue in zip(xList,yList):
        andResults.append(and_(xValue,yValue))
    return andResults

# Simply negates argument
def not_(argument): 
    # If x is true (1) it outputs a (0) and vice versa
    return if_(argument, 0, 1)

# Negates all arguments in a list
def NOT(argumentList):
    negatedList = []
    for argument in argumentList:
        negatedList.append(not_(argument))
    return negatedList

# Exclusive OR - XOR
# Return true if either x or y is true but not both at the same time
def xor(xValue, yValue): 
    # If x is true it outputs the oposite of y, so it outputs true only if x is true but y aren't
    # Else if x is false, then it outputs the unchanged value of y, acomplishing the same result as above
    return if_(xValue, not_(yValue), yValue)

# Similar to the AND function, it checks the XOR relation of every tuple within x and y cartesian product
def XOR(xList, yList):
    xorResults = []
    for xValue, yValue in zip(xList, yList):
        xorResults.append(xor(xValue,yValue))
    return xorResults

# A XOR Gate with multiple inputs acts in a way to output true if and only if there are an odd number of true arguments between the inputs
def xorxor(xValue, yValue, zValue): 
    # As with every other logic gate, if you append a two input gate to the result of another two input gate it acts as a three input one
    return xor(xValue, xor(yValue, zValue))

# Similar to the AND and XOR functions, it checks the XORXOR relation of every tuple within x, y and z cartesian product
def XORXOR(xList, yList, zList):
    xorxorResults = []
    for xValue, yValue, zValue in zip(xList, yList, zList):
        xorxorResults.append(xorxor(xValue, yValue, zValue))
    return xorxorResults

# Get the majority of results, i.e., if 2 or more of three values are the same 
def maj(xValue, yValue, zValue):
    if xValue == yValue or xValue == zValue:
        return xValue
    else:
        return yValue

# Rotate the bitstream in a circle such that empty spots are filled with bits shifted off the other end
def rotateRight(bitstream, amount_of_rotation):
    last_digit_index = len(bitstream) - 1
    newBitstream = bitstream.copy() # <====== WILL PROBABLY NEED A BETTER WAY IN C
    for rotation in range(0, amount_of_rotation):
        newBitstream.insert(0, newBitstream[last_digit_index])
        newBitstream.pop()
    return newBitstream

# Shifts the bitstream to the right (LSR) and fills empty spots with zeros
def shiftRight(bitstream, amount_of_shifting):
    newBitstream = bitstream.copy() # <====== WILL PROBABLY NEED A BETTER WAY IN C
    for i in range(0, amount_of_shifting):
        newBitstream.insert(0, 0)
        newBitstream.pop()
    return newBitstream

# Full binary adder - takes two bitstreams and adds them
def add(xBitstream, yBitstream):
  length = len(xBitstream)
  # It creates a result list with the size of the bitstream
  result = list(range(length))
  # Initial input needs to not have a carry over
  carryOver = 0
  # This loops sweeps our bitstream starting from the LSB (least significant bit)
  for index in range(length-1,-1,-1):
    # Add the input bits with a double xor gate
    result[index] = xorxor(xBitstream[index], yBitstream[index], carryOver)
    # Carry over bit is equal the most represented value, e.g., output = 0,1,0 means that 0 is the carry over bit
    carryOver = maj(xBitstream[index], yBitstream[index], carryOver)
  return result