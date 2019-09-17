#!/usr/bin/env python3

#######################################
#   Homework Number:    HW02
#   Name:               Gavin Neyman
#   ECN Login:          gneyman
#   Due Date:           1/31/2019
########################################

import sys
...

# Zn is a field if and only if n is prime so we determine if n is prime then return the appropriate string
def FieldOrRing(n):
    if n > 1:
        i = 2
        while i < n:
            if (n % i) == 0:
                return "Ring"
            else:
                i+=1
        return "Field"
    else:
        return "Ring"

# Get user input and print results
n = input("Please input an integer to test if its a field or a ring in modulo arithmatic: ")
print("Z sub", n, "constitues a", FieldOrRing(int(n)))
