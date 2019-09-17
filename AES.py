#!/usr/bin/env python3

#######################################
#   Homework Number:    HW04
#   Name:               Gavin Neyman
#   ECN Login:          gneyman
#   Due Date:           2/4/2019
########################################

import sys
from BitVector import *
...

AES_modulus = BitVector(bitstring='100011011')
subBytesTable = []
invSubBytesTable = []

def getKey():
    # Open the key file and read in the text
    keyFile = open("key.txt")
    key = keyFile.read()
    keyFile.close()
    # Create a bitvector for the key
    key_bv = BitVector(textstring=key)
    return key_bv

def getMessage():
    # Read and store the message from the designated file
    messageFile = open("message.txt")
    message = messageFile.read()
    messageFile.close()
    message_bv = BitVector(textstring=message)
    # Pad the message input so that it fits in the 16-byte block size
    padSize = len(message_bv) % 128
    message_bv.pad_from_right(padSize)
    return message_bv

def getEncryptedMessage():
    # Read from the encrypted file and convert it to a bitarray
    encryptFile = open("encrypt.txt")
    cypher_hex = encryptFile.read()
    encryptFile.close()
    cypher_bv = BitVector(hexstring=cypher_hex)
    return cypher_bv

def genTables():
    # Create the tables used for look ups in the sub-byte step
    c = BitVector(bitstring='01100011')
    d = BitVector(bitstring='00000101')
    for i in range(0, 256):
        # For the encryption SBox
        a = BitVector(intVal = i, size=8).gf_MI(AES_modulus, 8) if i != 0 else BitVector(intVal=0)
        # For bit scrambling for the encryption SBox entries:
        a1,a2,a3,a4 = [a.deep_copy() for x in range(4)]
        a ^= (a1 >> 4) ^ (a2 >> 5) ^ (a3 >> 6) ^ (a4 >> 7) ^ c
        subBytesTable.append(int(a))
        # For the decryption Sbox:
        b = BitVector(intVal = i, size=8)
        # For bit scrambling for the decryption SBox entries:
        b1,b2,b3 = [b.deep_copy() for x in range(3)]
        b = (b1 >> 2) ^ (b2 >> 5) ^ (b3 >> 7) ^ d
        check = b.gf_MI(AES_modulus, 8)
        b = check if isinstance(check, BitVector) else 0
        invSubBytesTable.append(int(b))

def g(keyword, round_constant, byte_sub_table):
    # one-byte left circular rotation
    rotated_word = keyword.deep_copy()
    for i in range(0, 8):
        rotated_word.circular_rotate_left_by_one()
    # Preform byte substitution
    newword = BitVector(size=0)
    for i in range(4):
        newword += BitVector(intVal=byte_sub_table[rotated_word[8 * i:8 * i + 8].intValue()], size=8)
    # XOR with round constant
    newword[:8] ^= round_constant
    round_constant = round_constant.gf_multiply_modular(BitVector(intVal=0x02), AES_modulus, 8)
    return newword, round_constant

def gen_subbytes_table():
    # Generate the table used to generate the key schedual as seen in lecture
    subBytesTable = []
    c = BitVector(bitstring='01100011')
    for i in range(0, 256):
        a = BitVector(intVal = i, size=8).gf_MI(AES_modulus, 8) if i != 0 else BitVector(intVal=0)
        a1,a2,a3,a4 = [a.deep_copy() for x in range(4)]
        a ^= (a1 >> 4) ^ (a2 >> 5) ^ (a3 >> 6) ^ (a4 >> 7) ^ c
        subBytesTable.append(int(a))
    return subBytesTable

def getRoundKeys(key_bv):
    # Get the key scedual as seen in the lecture notes
    byte_sub_table = gen_subbytes_table()
    key_words = [None for i in range(60)]
    round_constant = BitVector(intVal=0x01, size=8)
    for i in range(8):
        key_words[i] = key_bv[i * 32: i * 32 + 32]
    for i in range(8, 60):
        if i % 8 == 0:
            kwd, round_constant = g(key_words[i - 1], round_constant, byte_sub_table)
            key_words[i] = key_words[i - 8] ^ kwd
        elif (i - (i // 8) * 8) < 4:
            key_words[i] = key_words[i - 8] ^ key_words[i - 1]
        elif (i - (i // 8) * 8) == 4:
            key_words[i] = BitVector(size=0)
            for j in range(4):
                key_words[i] += BitVector(intVal=
                                          byte_sub_table[key_words[i - 1][8 * j:8 * j + 8].intValue()], size=8)
            key_words[i] ^= key_words[i - 8]
        elif ((i - (i // 8) * 8) > 4) and ((i - (i // 8) * 8) < 8):
            key_words[i] = key_words[i - 8] ^ key_words[i - 1]
        else:
            sys.exit("error in key scheduling algo for i = %d" % i)
    key_schedule = []
    for word_index, word in enumerate(key_words):
        keyword_in_ints = []
        for i in range(4):
            keyword_in_ints.append(word[i * 8:i * 8 + 8].intValue())
        key_schedule.append(keyword_in_ints)
    num_rounds = 14
    round_keys = [None for i in range(num_rounds + 1)]
    for i in range(num_rounds + 1):
        round_keys[i] = (key_words[i * 4] + key_words[i * 4 + 1] + key_words[i * 4 + 2] + key_words[i * 4 + 3]).get_bitvector_in_hex()
    return round_keys

def getMatrix(bitVec):
    # Transform 128 bits into a 4x4 matrix
    if type(bitVec) == list:
        bitVec = BitVector(bitlist=bitVec)
    matrix = [[None for i in range(0,4)] for j in range(0,4)]
    i = 0
    while i < 4:
        j=0
        while j < 4:
            value = bitVec[j * 8 + 32 * i:j * 8 + 8 + 32 * i]
            matrix[j][i] = value
            j+=1
        i+=1
    return matrix

def getBlocks(message):
    # Split the input up into 128 bit blocks and pad the final block with 0s if it is not exactly 128
    bitList = [None for i in range(0, 128)]
    i = 0
    blocks = []
    for bit in message:
        bitList[i] = bit
        i += 1
        if i == 128:
            i = 0
            blocks.append(BitVector(bitlist = bitList))
            bitList = [None for i in range(0, 128)]
    return blocks

def matrixXOR(mat1, mat2):
    # Xor two 4x4 matricies element by element
    cypher = [[None for i in range(0, 4)] for j in range(0, 4)]
    i = 0
    while i < len(mat1):
        j = 0
        while j < len(mat1[0]):
            cypher[i][j] = mat1[i][j] ^ mat2[i][j]
            j+=1
        i+=1
    return cypher

def shiftRows(stateArray):
    # Shift the rows of the state array
    i = 0
    while i < len(stateArray):
        j = 0
        shiftedRow = [[],[],[],[]]
        while j < len(stateArray[0]):
            shiftedRow[j] = stateArray[i][(j+i)%4]
            j+=1
        stateArray[i] = shiftedRow
        i+=1
    return stateArray

def mixColumns(stateArray):
    # Mix the columns of the state array
    mixedMatrix = [[0 for i in range(0, 4)] for j in range(0, 4)]
    i = 0
    while i < len(stateArray):
        j = 0
        while j < len(stateArray[0]):
            s1_bv = stateArray[i % 4][j].gf_multiply_modular(BitVector(intVal=0x02), AES_modulus, 8)
            if len(s1_bv) < 8:
                s1_bv.pad_from_left(8 - len(s1_bv))

            s2_bv = stateArray[(i+1) % 4][j].gf_multiply_modular(BitVector(intVal=0x03), AES_modulus, 8)
            if len(s1_bv) < 8:
                s2_bv.pad_from_left(8 - len(s2_bv))

            mixedMatrix[i][j] = s1_bv ^ s2_bv ^ stateArray[(i+2)%4][j] ^ stateArray[(i+3)%4][j]
            j+=1
        i+=1
    return mixedMatrix

def encrypt(roundKeys, message):
    cypher = []
    # Split the plaintext into 128 bit blocks
    blocks = getBlocks(message)
    # Preform encryption on each block
    for block in blocks:
        # Initial XOR
        stateMatrix = getMatrix(block)
        roundMatrix = getMatrix(roundKeys[0])
        stateMatrix = matrixXOR(stateMatrix, roundMatrix)
        # Loop for 14 rounds starting at round 1
        for i in range(1, 15):
            # Byte Substitution
            for line in stateMatrix:
                z = 0
                while z < len(line):
                    [left, right] = line[z].divide_into_two()
                    row = int(left.get_bitvector_in_hex(), 16)
                    column = int(right.get_bitvector_in_hex(), 16)
                    # tableValue = subBytesTable[row][column]
                    tableValue = subBytesTable[16 * row + column]
                    line[z] = BitVector(intVal=tableValue)
                    if len(line[z]) != 8:
                        line[z].pad_from_left(8 - len(line[z]))
                    z+=1
            # Shift Rows
            stateMatrix = shiftRows(stateMatrix)
            # Mix columns unless it is the last round
            if i != 14:
                stateMatrix = mixColumns(stateMatrix)
            # Add round key
            roundMatrix = getMatrix(roundKeys[i])
            stateMatrix = matrixXOR(stateMatrix, roundMatrix)
        cypher.append(stateMatrix)
    output = open("encrypt.txt", 'w')
    for block in cypher:
        i = 0
        while i < len(block[0]):
            j = 0
            while j < len(block):
                printable = block[j][i].get_bitvector_in_hex()
                output.write(printable)
                j+=1
            i+=1
    output.close()
    return

def invShiftRows(stateArray):
    # Preform shifts to undo the row shifts of encryption
    i = 0
    while i < len(stateArray):
        j = 0
        shiftedRow = [[],[],[],[]]
        while j < len(stateArray[0]):
            shiftedRow[j] = stateArray[i][(j-i)%4]
            j+=1
        stateArray[i] = shiftedRow
        i+=1
    return stateArray

def invMixColumns(stateArray):
    # Preform action to undo the column mixing of the encryption process
    mixedMatrix = [[0 for i in range(0, 4)] for j in range(0, 4)]
    i = 0
    while i < len(stateArray):
        j = 0
        while j < len(stateArray[0]):
            s1_bv = stateArray[i % 4][j].gf_multiply_modular(BitVector(intVal=0x0E), AES_modulus, 8)
            if len(s1_bv) < 8:
                s1_bv.pad_from_left(8 - len(s1_bv))

            s2_bv = stateArray[(i + 1) % 4][j].gf_multiply_modular(BitVector(intVal=0x0B), AES_modulus, 8)
            if len(s2_bv) < 8:
                s2_bv.pad_from_left(8 - len(s2_bv))

            s3_bv = stateArray[(i + 2) % 4][j].gf_multiply_modular(BitVector(intVal=0x0D), AES_modulus, 8)
            if len(s3_bv) < 8:
                s3_bv.pad_from_left(8 - len(s3_bv))

            s4_bv = stateArray[(i + 3) % 4][j].gf_multiply_modular(BitVector(intVal=0x09), AES_modulus, 8)
            if len(s4_bv) < 8:
                s4_bv.pad_from_left(8 - len(s4_bv))

            mixedMatrix[i][j] = s1_bv ^ s2_bv ^ s3_bv ^ s4_bv
            j += 1
        i += 1
    return mixedMatrix

def decrypt(roundKeys, encrypted_bv):
    cypher = []
    # Split the encrypted text up into 128 bit blocks
    blocks = getBlocks(encrypted_bv)
    for block in blocks:
        # Get the state matrix
        stateMatrix = getMatrix(block)
        # Inital xor with final round key
        roundMatrix = getMatrix(roundKeys[14])
        stateMatrix = matrixXOR(stateMatrix, roundMatrix)
        # Loop for 14 rounds starting at round 0
        for i in range(0, 14):
            # Inverse Shift Rows
            stateMatrix = invShiftRows(stateMatrix)
            # Inverse Byte Substitution
            for line in stateMatrix:
                z = 0
                while z < len(line):
                    [left, right] = line[z].divide_into_two()
                    row = int(left.get_bitvector_in_hex(), 16)
                    column = int(right.get_bitvector_in_hex(), 16)
                    tableValue = invSubBytesTable[16 * row + column]
                    line[z] = BitVector(intVal=tableValue)
                    if len(line[z]) != 8:
                        line[z].pad_from_left(8 - len(line[z]))
                    z+=1
            # Add round key
            shift = len(roundKeys) - i - 2
            roundMatrix = getMatrix(roundKeys[shift])
            stateMatrix = matrixXOR(stateMatrix, roundMatrix)
            # Inverse Column Mixing except for the final round
            if i != 13:
                stateMatrix = invMixColumns(stateMatrix)
        cypher.append(stateMatrix)
    # Print each byte in each block of decrepted text
    output = open("decrypt.txt", 'w')
    for block in cypher:
        i = 0
        while i < len(block[0]):
            j = 0
            while j < len(block):
                printable = block[j][i].get_bitvector_in_ascii()
                output.write(printable)
                j+=1
            i+=1
    output.close()
    return

# Read in the encryption key and convert it to bits
key_bv = getKey()
# Read in the plain text message and convert it to bits
message_bv = getMessage()
# Get the expanded round keys
hexRoundKeys = getRoundKeys(key_bv)
# Convert the hex key to binary
roundKeys = []
for hexKey in hexRoundKeys:
    key = BitVector(hexstring=hexKey)
    roundKeys.append(key)
# Get the sub tables needed for byte-substitution
genTables()
# Begin the encryption process
encrypt(roundKeys, message_bv)
# Open file with hex encryption and convert it to bits
cypher_bv = getEncryptedMessage()
# Begin the decryption process
decrypt(roundKeys, cypher_bv)