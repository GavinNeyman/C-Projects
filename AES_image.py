#!/usr/bin/env python3

#######################################
#   Homework Number:    HW05
#   Name:               Gavin Neyman
#   ECN Login:          gneyman
#   Due Date:           2/19/2019
########################################

import sys
import time
from BitVector import *

...

AES_modulus = BitVector(bitstring='100011011')
subBytesTable = []
invSubBytesTable = []


def genTables():
    # Create the tables used for look ups in the sub-byte step
    c = BitVector(bitstring='01100011')
    d = BitVector(bitstring='00000101')
    for i in range(0, 256):
        # For the encryption SBox
        a = BitVector(intVal=i, size=8).gf_MI(AES_modulus, 8) if i != 0 else BitVector(intVal=0)
        # For bit scrambling for the encryption SBox entries:
        a1, a2, a3, a4 = [a.deep_copy() for x in range(4)]
        a ^= (a1 >> 4) ^ (a2 >> 5) ^ (a3 >> 6) ^ (a4 >> 7) ^ c
        subBytesTable.append(int(a))
        # For the decryption Sbox:
        b = BitVector(intVal=i, size=8)
        # For bit scrambling for the decryption SBox entries:
        b1, b2, b3 = [b.deep_copy() for x in range(3)]
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
        a = BitVector(intVal=i, size=8).gf_MI(AES_modulus, 8) if i != 0 else BitVector(intVal=0)
        a1, a2, a3, a4 = [a.deep_copy() for x in range(4)]
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
        round_keys[i] = (key_words[i * 4] + key_words[i * 4 + 1] + key_words[i * 4 + 2] + key_words[
            i * 4 + 3]).get_bitvector_in_hex()
    return round_keys


def getMatrix(bitVec):
    # Transform 128 bits into a 4x4 matrix
    if type(bitVec) == list:
        bitVec = BitVector(bitlist=bitVec)
    matrix = [[None for i in range(0, 4)] for j in range(0, 4)]
    i = 0
    while i < 4:
        j = 0
        while j < 4:
            value = bitVec[j * 8 + 32 * i:j * 8 + 8 + 32 * i]
            matrix[j][i] = value
            j += 1
        i += 1
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
            blocks.append(BitVector(bitlist=bitList))
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
            j += 1
        i += 1
    return cypher


def shiftRows(stateArray):
    # Shift the rows of the state array
    i = 0
    while i < len(stateArray):
        j = 0
        shiftedRow = [[], [], [], []]
        while j < len(stateArray[0]):
            shiftedRow[j] = stateArray[i][(j + i) % 4]
            j += 1
        stateArray[i] = shiftedRow
        i += 1
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

            s2_bv = stateArray[(i + 1) % 4][j].gf_multiply_modular(BitVector(intVal=0x03), AES_modulus, 8)
            if len(s1_bv) < 8:
                s2_bv.pad_from_left(8 - len(s2_bv))

            mixedMatrix[i][j] = s1_bv ^ s2_bv ^ stateArray[(i + 2) % 4][j] ^ stateArray[(i + 3) % 4][j]
            j += 1
        i += 1
    return mixedMatrix


def AES(roundKeys, message_bv):
    cypher = []
    padSize = len(message_bv) % 128
    message_bv.pad_from_left(padSize)
    # Split the plaintext into 128 bit blocks
    blocks = getBlocks(message_bv)
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
                    tableValue = subBytesTable[16 * row + column]
                    line[z] = BitVector(intVal=tableValue)
                    if len(line[z]) != 8:
                        line[z].pad_from_left(8 - len(line[z]))
                    z += 1
            # Shift Rows
            stateMatrix = shiftRows(stateMatrix)
            # Mix columns unless it is the last round
            if i != 14:
                stateMatrix = mixColumns(stateMatrix)
            # Add round key
            roundMatrix = getMatrix(roundKeys[i])
            stateMatrix = matrixXOR(stateMatrix, roundMatrix)
        cypher.append(stateMatrix)
    encryptedBytes = []
    for block in cypher:
        i = 0
        while i < len(block[0]):
            j = 0
            while j < len(block):
                for bit in block[j][i]:
                    encryptedBytes.append(bit)
                j += 1
            i += 1
    return BitVector(bitlist=encryptedBytes)


def x931(v0, dt, totalNum, key_file='key.txt'):
    # Initalize values and bitvectors
    f = open(key_file, 'r')
    key = f.read()
    key_bv = BitVector(textstring=key)
    f.close()
    vj_bv = BitVector(textstring=v0)
    dt_bv = BitVector(intVal=dt)
    if len(dt_bv) < 64:
        dt_bv.pad_from_left(64 - len(dt_bv))
    randomNums = []
    i = 0

    # Generate key schedule for AES
    hexRoundKeys = getRoundKeys(key_bv)
    # Convert the hex key to binary
    roundKeys = []
    for hexKey in hexRoundKeys:
        key = BitVector(hexstring=hexKey)
        roundKeys.append(key)
    # Get the sub tables needed for byte-substitution
    genTables()

    # Generate 'totalNum' random numbers
    while i < totalNum:
        # Put the date and time through an AES encryption
        node1 = AES(roundKeys, dt_bv)
        # XOR the seed vector with the output of AES(date and time)
        node2 = vj_bv ^ node1
        # Generate a random number
        r = AES(roundKeys, node2)
        # XOR the random number result and AES(date and time)
        node3 = node1 ^ r
        # Determine the seed vector for the next random number
        vj_bv = AES(roundKeys, node3)
        [throw_away, vj_bv] = vj_bv.divide_into_two()
        # Preform conversions
        [throw_away, r] = r.divide_into_two()
        randNum = r.int_val()
        randomNums.append(randNum)
        i += 1
    return randomNums


def ctr_aes_image(iv, image_file='image.ppm', out_file='enc_image.ppm', key_file='key.txt'):
    bv = BitVector(filename=image_file)
    # Extract the image header
    header = bv.read_bits_from_file(104)

    f = open(key_file, 'r')
    key = f.read()
    key_bv = BitVector(textstring=key)
    f.close()

    # Generate key schedule for AES
    hexRoundKeys = getRoundKeys(key_bv)
    # Convert the hex key to binary
    roundKeys = []
    for hexKey in hexRoundKeys:
        key = BitVector(hexstring=hexKey)
        roundKeys.append(key)

    iv.pad_from_left(128-len(iv))
    f = open(out_file, 'w', encoding="utf-8")
    f.write(header.get_bitvector_in_ascii())

    while bv.more_to_read:
        blockEncrypt = AES(roundKeys, iv)
        bitvec = bv.read_bits_from_file(128)
        xorResult = blockEncrypt^bitvec
        f.write(xorResult.get_bitvector_in_ascii())
        increment = iv.int_val()
        increment += 1
        iv = BitVector(intVal=increment)
        iv.pad_from_left(128 - len(iv))

    f.close()
    return


# Initialize 64-bit seed value
v0 = "ecepurdu"

# Get the date and time
dt = time.time()
dt = dt * 1000000
dt = int(dt)

# Call the ANSI X931 function
iv = x931(v0, dt, 1, "key.txt")
ctr_aes_image(BitVector(intVal=iv[0]))