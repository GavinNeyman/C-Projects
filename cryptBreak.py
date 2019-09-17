#!/usr/bin/env python3

########################
#   Homework Number: 01
#   Name: Gavin Neyman
#   ECN Login: gneyman
#   Due Date: 1/17/19
########################

import sys
from itertools import *
from BitVector import *
...

# Known PassPhrase and Blocksize
PassPhrase = "Hopes and dreams of a million years"
BLOCKSIZE = 16
numbytes = BLOCKSIZE // 8
# Assuming the key is a letter/number combination like the cyphertext
KeyChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"

# Read in the encrypted text from the file and convert it to a bitvector
# FILEIN = open(sys.argv[1])
FILEIN = open("encrypted.txt", "r")
encrypted_bv = BitVector(hexstring=FILEIN.read())
FILEIN.close()

# Convert the passphrase into a bit array
bv_iv = BitVector(bitlist=[0]*BLOCKSIZE)
for i in range(0, len(PassPhrase)//numbytes):
    textstr = PassPhrase[i*numbytes:(i+1)*numbytes]
    bv_iv ^= BitVector(textstring=textstr)

# Keep trying keys until the correct one is found
while True:
    # Create a list of possible keys
    PossibleKeys = product(KeyChars, repeat=16)
    # Loop through each possible key
    for key in PossibleKeys:
        # Convert the key to a bit array
        key_bv = BitVector(bitlist=[0]*BLOCKSIZE)
        for i in range(0, BLOCKSIZE//numbytes):
            keyblock = key[i*numbytes:(i+1)*numbytes]
            key_bv ^= BitVector(textstring=keyblock)

        # Bitvector to hold decoded text
        msg_decrypted_bv = BitVector(size=0)

        # Decrypt using XORing similar to its encryption
        previous_decrypted_block = bv_iv
        for i in range(0, len(encrypted_bv)//BLOCKSIZE):
            bv = encrypted_bv[i*BLOCKSIZE:(i+1)*BLOCKSIZE]
            temp = bv.deep_copy()
            bv ^= previous_decrypted_block
            previous_decrypted_block = temp
            bv ^= key_bv
            msg_decrypted_bv += bv

        # Convert the decrypted bit vector into text
        plain = msg_decrypted_bv.getTextFromBitVector()

        # Check to see if the known phrase "Cormac McCarthy" is in the decoded plain text
        if ("Cormac" in plain) and ("McCarthy" in plain):
            PrintableKey = "".join(key)
            print("Key = ", PrintableKey)
            print(plain)
            break
