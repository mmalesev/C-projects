#!/usr/bin/env python

### hw2_starter.py

import sys
import os
import BitVector
from BitVector import *

expansion_permutation = [31, 0, 1, 2, 3, 4, 3, 4, 5, 6, 7, 8, 7, 8, 9, 10, 11, 12, 11, 12, 13, 14, 15, 16, 15, 16, 17, 18, 19, 20, 19, 20, 21, 22, 23, 24, 23, 24, 25, 26, 27, 28, 27, 28, 29, 30, 31, 0]
## permutation matrices taken from the generate_round_keys.py
key_permutation_1 = [56,48,40,32,24,16,8,0,57,49,41,33,25,17,
                      9,1,58,50,42,34,26,18,10,2,59,51,43,35,
                     62,54,46,38,30,22,14,6,61,53,45,37,29,21,
                     13,5,60,52,44,36,28,20,12,4,27,19,11,3]

key_permutation_2 = [13,16,10,23,0,4,2,27,14,5,20,9,22,18,11,
                      3,25,7,15,6,26,19,12,1,40,51,30,36,46,
                     54,29,39,50,44,32,47,43,48,38,55,33,52,
                     45,41,49,35,28,31]

shifts_for_rounds = [1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1]

s_boxes = {i:None for i in range(8)}
## s boxes taken from the lecture slides
s_boxes[0] = [ [14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7],
[0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8],
[4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0],
[15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13] ]

s_boxes[1] = [ [15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10],
[3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5],
[0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15],
[13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9] ]

s_boxes[2] = [ [10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8],
[13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1],
[13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7],
[1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12] ]

s_boxes[3] = [ [7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15],
[13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9],
[10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4],
[3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14] ]

s_boxes[4] = [ [2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9],
[14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6],
[4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14],
[11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3] ]

s_boxes[5] = [ [12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11],
[10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8],
[9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6],
[4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13] ]

s_boxes[6] = [ [4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1],
[13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6],
[1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2],
[6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12] ]

s_boxes[7] = [ [13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7],
[1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2],
[7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8],
[2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11] ]

## generated as output of random.sample(range(32), 32)
pbox = [3, 22, 29, 0, 15, 19, 8, 17, 27, 10, 26, 16, 2, 21, 24, 28, 1, 14, 4, 9, 20, 13, 6, 11, 5, 25, 18, 12, 7, 30, 31, 23]

def get_encryption_key():
    # getting the key as first 8 bytes from the file key.txt
    with open("key.txt") as f:
        key = f.read(8)
    key_bits = BitVector(textstring = key)
    return_bits = key_bits.permute(key_permutation_1)
    return return_bits

def extract_round_key(original_key):
    # making a list of 16 different keys for 16 different rounds of the Feistel structure
    round_keys = []
    # making a deep key to keep the original key untouched
    key = original_key.deep_copy()
    for round in range(16):
        [left, right] = key.divide_into_two()
        left << shifts_for_rounds[round]
        right << shifts_for_rounds[round]
        key = left + right
        round_key = key.permute(key_permutation_2)
        round_keys.append(round_key)
    return round_keys

def s_box_sub(expanded_bits):
    output = BitVector(size=32)
    for i in range(8):
        #determining row and column in the s-box for the element
        row = int(BitVector( bitlist = [expanded_bits[6*i], expanded_bits[6*i+5]]))
        col = int(expanded_bits[i*6+1 : 6*i+5])
        # getting the integer value from the appropriate s-box
        new_value = s_boxes[i][row][col]
        new_value_bits = BitVector(intVal = new_value, size = 4)
        output[i*4:(i+1)*4] = new_value_bits
    return output


def encrypt():
    key = get_encryption_key()
    #getting the list of the round keys
    round_key = extract_round_key( key )
    #plain text taken from the message.txt
    bv = BitVector( filename = 'message.txt' )
    #delete any existing file named "encrypted.txt"
    if(os.path.exists('encrypted.txt')):
       os.remove('encrypted.txt')
    #open the file for writing
    FILEOUT = open('encrypted.txt', 'a')
    while (bv.more_to_read):
        #take 64 bits
        bitvec = bv.read_bits_from_file( 64 )
        if (len(bitvec) > 0):
            for i in range(16):
                #split
                [LE, RE] = bitvec.divide_into_two()
                #permute
                newRE = RE.permute( expansion_permutation )
                #xor
                out_xor = newRE ^ round_key[i]
                #diffusion
                out_sub = s_box_sub(out_xor)
                #confusion
                out_perm = out_sub.permute(pbox)
                #apply the Feistel function
                RE_modified = out_perm ^ LE
                #join the vector
                bitvec = RE + RE_modified
            #last exchange
            [LE, RE] = bitvec.divide_into_two()
            encrypted_text = RE + LE
            encrypted_text.write_to_file(FILEOUT)
    FILEOUT.close()

#decryption is exact the same as encryption, the only thing that is different is argument for the for loop
def decrypt():
    key = get_encryption_key()
    round_key = extract_round_key( key )
    bv = BitVector( filename = 'encrypted.txt' )
    if(os.path.exists('decrypted.txt')):
       os.remove('decrypted.txt')
    FILEOUT = open('decrypted.txt', 'ab')
    while (bv.more_to_read):
        bitvec = bv.read_bits_from_file( 64 )
        if (len(bitvec) > 0):
            for i in range(15, -1, -1):
                [LE, RE] = bitvec.divide_into_two()
                newRE = RE.permute( expansion_permutation )
                out_xor = newRE ^ round_key[i]
                out_sub = s_box_sub(out_xor)
                out_perm = out_sub.permute(pbox)
                RE_modified = out_perm ^ LE
                bitvec = RE + RE_modified
            [LE, RE] = bitvec.divide_into_two()
            encrypted_text = RE + LE
            encrypted_text.write_to_file(FILEOUT)
    FILEOUT.close()

encrypt()
decrypt()
