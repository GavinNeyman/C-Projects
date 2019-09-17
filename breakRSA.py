#!/usr/bin/env python3

#######################################
#   Homework Number:    HW06
#   Name:               Gavin Neyman
#   ECN Login:          gneyman
#   Due Date:           2/26/2019
########################################

import sys
import random
import numpy as np
from BitVector import *
...

def solve_pRoot(p,y):
    p = int(p)
    y = int(y)
    # Initial guess for xk
    try:
        xk = int(pow(y,1.0/p))
    except:
        # Necessary for larger value of y
        # Approximate y as 2^a * y0
        y0 = y
        a = 0
        while y0 > sys.float_info.max:
            y0 = y0 >> 1
            a += 1
        # log xk = log2 y / p
        # log xk = (a + log2 y0) / p
        xk = int(pow(2.0, ( a + np.log2(float(y0)) )/ p ))

    # Solve for x using Newton's Method
    err_k = int(pow(xk,p))-y
    while abs(err_k) > 1:
        gk = p*int(pow(xk,p-1))
        err_k = int(pow(xk,p))-y
        xk = int(-err_k/gk) + xk
    return xk

class PrimeGenerator( object ):

    def __init__( self, **kwargs ):
        bits = debug = None
        if 'bits' in kwargs  :     bits = kwargs.pop('bits')
        if 'debug' in kwargs :     debug = kwargs.pop('debug')
        self.bits            =     bits
        self.debug           =     debug
        self._largest        =     (1 << bits) - 1

    def set_initial_candidate(self):
        candidate = random.getrandbits( self.bits )
        if candidate & 1 == 0: candidate += 1
        candidate |= (1 << self.bits-1)
        candidate |= (2 << self.bits-3)
        self.candidate = candidate

    def set_probes(self):
        self.probes = [2,3,5,7,11,13,17]

    def test_candidate_for_prime(self):
        p = self.candidate
        if p == 1: return 0
        if p in self.probes:
            self.probability_of_prime = 1
            return 1
        if any([p % a == 0 for a in self.probes]): return 0
        k, q = 0, self.candidate-1
        while not q&1:
            q >>= 1
            k += 1
        if self.debug: print("q = %d  k = %d" % (q,k))
        for a in self.probes:
            a_raised_to_q = pow(a, q, p)
            if a_raised_to_q == 1 or a_raised_to_q == p-1: continue
            a_raised_to_jq = a_raised_to_q
            primeflag = 0
            for j in range(k-1):
                a_raised_to_jq = pow(a_raised_to_jq, 2, p)
                if a_raised_to_jq == p-1:
                    primeflag = 1
                    break
            if not primeflag: return 0
        self.probability_of_prime = 1 - 1.0/(4 ** len(self.probes))
        return self.probability_of_prime

    def findPrime(self):
        self.set_initial_candidate()
        if self.debug:  print("    candidate is: %d" % self.candidate)
        self.set_probes()
        if self.debug:  print("    The probes are: %s" % str(self.probes))
        max_reached = 0
        while 1:
            if self.test_candidate_for_prime():
                if self.debug:
                    print("Prime number: %d with probability %f\n" %
                          (self.candidate, self.probability_of_prime) )
                break
            else:
                if max_reached:
                    self.candidate -= 2
                elif self.candidate >= self._largest - 2:
                    max_reached = 1
                    self.candidate -= 2
                else:
                    self.candidate += 2
                if self.debug:
                    print("    candidate is: %d" % self.candidate)
        return self.candidate

def gcd(a,b):
    while b:
        a, b = b, a%b
    return a

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
            adder = BitVector(bitlist = bitList)
            adder.pad_from_left(128)
            blocks.append(adder)
            bitList = [None for i in range(0, 128)]
    return blocks

def crack(fileIn, fileOut, e):
    # Open input file and read message
    input_file = open(fileIn, 'r')
    message = input_file.read()
    input_file.close()
    M = BitVector(textstring=message)
    generator = PrimeGenerator(bits=128)
    i = 0
    nList = [None, None, None]
    cypherList = []
    # Create three simulated sending of the message as encrypted text
    while i < 3:
        searching = True
        # Seach for prime numbers to use for p and q until the requirements are met
        while searching:
            # Generate primes p and q
            q = generator.findPrime()
            p = generator.findPrime()
            # Convert the primes into bitvectors
            q_bv = BitVector(intVal=q)
            p_bv = BitVector(intVal=p)
            # p and q can not be equal
            if p != q:
                # The first two bits of p and q must be 1
                if q_bv[0] and q_bv[1] and p_bv[0] and p_bv[1]:
                    # (p-1) and (q-1) must be coprime to e
                    if gcd((p-1), e) == 1 and gcd((q-1), e) == 1:
                        searching = False
        # Store the n values for later reference
        n = p * q
        nList[i] = n
        # Encrypt or decrypt the message similar to rsa.py
        if len(M) % 128 != 0:
            M.pad_from_left(128 - (len(M) % 128))
        blocks = getBlocks(M)
        cypherBlocks = []
        for block in blocks:
            M_int = block.intValue()
            C = pow(M_int, e, n)
            C_bv = BitVector(intVal=C)
            if len(C_bv) < 256:
                C_bv.pad_from_left(256 - len(C_bv))
            cypher_hex = C_bv.get_bitvector_in_hex()
            cypherBlocks.append(cypher_hex)
        # Store the encrypted message for later use
        cypherList.append("".join(cypherBlocks))
        i+=1
    # Implement the Chinese Remainder Theorum
    N = nList[0] * nList[1] * nList[2]
    C1 = BitVector(hexstring=cypherList[0])
    C2 = BitVector(hexstring=cypherList[1])
    C3 = BitVector(hexstring=cypherList[2])
    C = C1.intValue() * C2.intValue() * C3.intValue()
    # Solve M^3 % N = C for M
    M_cubed = C % N
    plaintext_M = solve_pRoot(3, M_cubed)
    #plaintext_M = int(M_cubed ** (1/3))
    plaintext_bv = BitVector(intVal=plaintext_M)
    # Pad so text is printable
    if len(plaintext_bv) % 128 != 0:
        plaintext_bv.pad_from_right(128-(len(plaintext_bv)%128))
    # Write output to file
    output_file = open(fileOut, 'w')
    output_file.write(plaintext_bv.get_bitvector_in_ascii())
    output_file.close()


e = 3
crack(sys.argv[1], sys.argv[2], e)
