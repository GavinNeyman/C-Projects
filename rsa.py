#!/usr/bin/env python3

#######################################
#   Homework Number:    HW06
#   Name:               Gavin Neyman
#   ECN Login:          gneyman
#   Due Date:           2/26/2019
########################################

import sys
import random
from BitVector import *
...

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

def getBlocks(message, crypt):
    # Split the input up into 128 bit blocks and pad the final block with 0s if it is not exactly 128
    if crypt == '-e':
        bitList = [None for i in range(0, 128)]
    elif crypt == '-d':
        bitList = [None for i in range(0, 256)]
    i = 0
    blocks = []
    for bit in message:
        if crypt == '-e':
            bitList[i] = bit
            i += 1
            if i == 128:
                i = 0
                adder = BitVector(bitlist = bitList)
                adder.pad_from_left(128)
                blocks.append(adder)
                bitList = [None for i in range(0, 128)]
        elif crypt == '-d':
            bitList[i] = bit
            i += 1
            if i == 256:
                i = 0
                blocks.append(BitVector(bitlist=bitList))
                bitList = [None for i in range(0, 256)]
    return blocks

def rsa(M, e, crypt, fileName):
    output_file = open(fileName, 'w')
    if crypt == '-e':
        generator = PrimeGenerator(bits=128)
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
        # Store p and q so they can be used later
        f = open("p.txt", 'w')
        f.write(str(p))
        f.close()
        f = open("q.txt", 'w')
        f.write(str(q))
        f.close()
    elif crypt == '-d':
        # Read p and q from their designated files
        f = open("p.txt", 'r')
        p = int(f.read())
        p_bv = BitVector(intVal=p)
        f.close()
        f = open("q.txt", 'r')
        q = int(f.read())
        q_bv = BitVector(intVal=q)
        f.close()
    # Determine the modulo and totient and their bitvector equivalent
    n = p * q
    n_bv = BitVector(intVal=n)
    totient = (p-1) * (q-1)
    totient_bv = BitVector(intVal=totient)
    # Compute d
    e_bv = BitVector(intVal=e)
    d_bv = e_bv.multiplicative_inverse(totient_bv)
    d = d_bv.intValue()
    # Encrypt or decrypt the message
    if crypt == '-e':
        if len(M) % 128 != 0:
            M.pad_from_left(128 - (len(M) % 128))
        blocks = getBlocks(M, '-e')
        for block in blocks:
            M_int = block.intValue()
            C = pow(M_int, e, n)
            C_bv = BitVector(intVal=C)
            if len(C_bv) < 256:
                C_bv.pad_from_left(256 - len(C_bv))
            output_file.write(C_bv.get_bitvector_in_hex())

    elif crypt == '-d':
        blocks = getBlocks(M, '-d')
        for block in blocks:
            C = block.intValue()
            # Compute the modular exponentiation
            Vp = pow(C, d, p)
            Vq = pow(C, d, q)
            qmodp = q_bv.multiplicative_inverse(p_bv)
            Xp = q * qmodp.intValue()
            pmodq = p_bv.multiplicative_inverse(q_bv)
            Xq = p * pmodq.intValue()
            Cd_mod_n = (Vp * Xp + Vq * Xq) % n
            # Remove the padded zeros
            M_bv = BitVector(intVal=Cd_mod_n)
            if len(M_bv) < 256:
                M_bv.pad_from_left(256 - len(M_bv) % 256)
            M_bv = M_bv[128:len(M_bv)]
            output_file.write(M_bv.get_bitvector_in_ascii())
    output_file.close()
    return


e = 65537
# Encryption
if sys.argv[1] == '-e':
    # Read the message and preform rsa encryption on it
    f = open(sys.argv[2], 'r')
    M = f.read()
    f.close()
    M_bv = BitVector(textstring=M)
    rsa(M_bv, e, sys.argv[1], sys.argv[3])
#Decryption
elif sys.argv[1] == '-d':
    # Read the cyphertext and preform rsa decryption on it
    encrypted_text = open(sys.argv[2], 'r')
    C = encrypted_text.read()
    encrypted_text.close()
    C_bv = BitVector(hexstring=C)
    rsa(C_bv, e, sys.argv[1], sys.argv[3])
