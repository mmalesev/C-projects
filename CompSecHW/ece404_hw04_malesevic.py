#!/usr/bin/env python2.7
import sys
import os
from BitVector import *

#Code used in this function adopted from the lecture notes
def genTables():
    AES_modulus = BitVector(bitstring='100011011')
    subBytesTable = [] # SBox for encryption
    invSubBytesTable = [] # SBox for decryption
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
    return [subBytesTable, invSubBytesTable]

def gee(keyword, round_constant, byte_sub_table):
    AES_modulus = BitVector(bitstring='100011011')
    rotated_word = keyword.deep_copy()
    rotated_word << 8
    newword = BitVector(size = 0)
    for i in range(4):
        newword += BitVector(intVal = byte_sub_table[rotated_word[8*i:8*i+8].intValue()], size = 8)
    newword[:8] ^= round_constant
    round_constant = round_constant.gf_multiply_modular(BitVector(intVal = 0x02), AES_modulus, 8)
    return newword, round_constant

def gen_key_schedule_128(key_bv):
    AES_modulus = BitVector(bitstring='100011011')
    byte_sub_table = gen_subbytes_table()
    # We need 44 keywords in the key schedule for 128 bit AES. Each keyword is 32-bits
    # wide. The 128-bit AES uses the first four keywords to xor the input block with.
    # Subsequently, each of the 10 rounds uses 4 keywords from the key schedule. We will
    # store all 44 keywords in the following list:
    key_words = [None for i in range(44)]
    round_constant = BitVector(intVal = 0x01, size=8)
    for i in range(4):
        key_words[i] = key_bv[i*32 : i*32 + 32]
    for i in range(4,44):
        if i%4 == 0:
            kwd, round_constant = gee(key_words[i-1], round_constant, byte_sub_table)
            key_words[i] = key_words[i-4] ^ kwd
        else:
            key_words[i] = key_words[i-4] ^ key_words[i-1]
    return key_words

def gen_subbytes_table():
    AES_modulus = BitVector(bitstring='100011011')
    subBytesTable = []
    c = BitVector(bitstring='01100011')
    for i in range(0, 256):
        a = BitVector(intVal = i, size=8).gf_MI(AES_modulus, 8) if i != 0 else BitVector(intVal=0)
        a1,a2,a3,a4 = [a.deep_copy() for x in range(4)]
        a ^= (a1 >> 4) ^ (a2 >> 5) ^ (a3 >> 6) ^ (a4 >> 7) ^ c
        subBytesTable.append(int(a))
    return subBytesTable

def sub_bytes(block, sBox):
    sub_block = BitVector(size = 0)
    for i in range(16):
        row = int(block[i*8 : i*8+4])
        col = int(block[i*8+4 : i*8+8])
        result = sBox[row*16+col]
        sub_block = sub_block + BitVector(intVal = result, size = 8)
    return sub_block

def row_shift(block):
    shifted_block = BitVector(size = 128)
    for i in range(16):
        if i < 4:
            shifted_block[i*8:i*8+8] = block[i*8:i*8+8]
        elif i == 4:
            shifted_block[(i+3)*8:(i+4)*8] = block[i*8:i*8+8]
        elif i < 8:
            shifted_block[(i-1)*8:i*8] = block[i*8:i*8+8]
        elif i < 10:
            shifted_block[(i+2)*8:(i+3)*8] = block[i*8:i*8+8]
        elif i < 12:
            shifted_block[(i-2)*8:(i-1)*8] = block[i*8:i*8+8]
        elif i == 15:
            shifted_block[(i-3)*8:(i-2)*8] = block[i*8:i*8+8]
        else:
            shifted_block[(i+1)*8:(i+2)*8] = block[i*8:i*8+8]
    return shifted_block

def inv_row_shift(block):
    shifted_block = BitVector(size = 128)
    for i in range(16):
        if i < 4:
            shifted_block[i*8:i*8+8] = block[i*8:i*8+8]
        elif i < 7:
            shifted_block[(i+1)*8:(i+2)*8] = block[i*8:i*8+8]
        elif i == 7:
            shifted_block[4*8:5*8] = block[i*8:i*8+8]
        elif i < 10:
            shifted_block[(i+2)*8:(i+3)*8] = block[i*8:i*8+8]
        elif i < 12:
            shifted_block[(i-2)*8:(i-1)*8] = block[i*8:i*8+8]
        elif i == 12:
            shifted_block[(i+3)*8:(i+4)*8] = block[i*8:i*8+8]
        else:
            shifted_block[(i-1)*8:i*8] = block[i*8:i*8+8]
    return shifted_block

def col_mix(block):
    mixed_block = BitVector(size = 128)
    modulus = BitVector(bitstring='100011011') # AES modulus
    n = 8
    two = BitVector(intVal = 2, size = 8)
    three = BitVector(intVal = 3, size = 8)
    for i in range(4):
        mixed_block[i*8:(i+1)*8] = block[(i)*8:(i+1)*8].gf_multiply_modular(two,modulus,n) ^ \
                                   block[(i+4)*8:(i+5)*8].gf_multiply_modular(three,modulus,n) ^ \
                                   block[(i+8)*8:(i+9)*8] ^ block[(i+12)*8:(i+13)*8]
    for i in range(4,8):
        mixed_block[i*8:(i+1)*8] = block[(i)*8:(i+1)*8].gf_multiply_modular(two,modulus,n) ^ \
                                   block[(i+4)*8:(i+5)*8].gf_multiply_modular(three,modulus,n) ^ \
                                   block[(i+8)*8:(i+9)*8] ^ block[(i-4)*8:(i-3)*8]
    for i in range(8,12):
        mixed_block[i*8:(i+1)*8] = block[(i)*8:(i+1)*8].gf_multiply_modular(two,modulus,n) ^ \
                                   block[(i+4)*8:(i+5)*8].gf_multiply_modular(three,modulus,n) ^ \
                                   block[(i-8)*8:(i-7)*8] ^ block[(i-4)*8:(i-3)*8]
    for i in range(12,16):
        mixed_block[i*8:(i+1)*8] = block[(i)*8:(i+1)*8].gf_multiply_modular(two,modulus,n) ^ \
                                   block[(i-12)*8:(i-11)*8].gf_multiply_modular(three,modulus,n)^\
                                   block[(i-8)*8:(i-7)*8] ^ block[(i-4)*8:(i-3)*8]
    return mixed_block

def inv_col_mix(block):
    mixed_block = BitVector(size = 128)
    modulus = BitVector(bitstring='100011011') # AES modulus
    n = 8
    e = BitVector(hexstring = "0e")
    b = BitVector(hexstring = "0b")
    d = BitVector(hexstring = "0d")
    nine = BitVector(intVal = 9, size = 8)
    for i in range(4):
        mixed_block[i*8:(i+1)*8] = block[(i)*8:(i+1)*8].gf_multiply_modular(e,modulus,n) ^ \
                                   block[(i+4)*8:(i+5)*8].gf_multiply_modular(b,modulus,n) ^ \
                                   block[(i+8)*8:(i+9)*8].gf_multiply_modular(d,modulus,n) ^ \
                                   block[(i+12)*8:(i+13)*8].gf_multiply_modular(nine,modulus,n)
    for i in range(4,8):
        mixed_block[i*8:(i+1)*8] = block[(i-4)*8:(i-3)*8].gf_multiply_modular(nine,modulus,n) ^ \
                                   block[(i)*8:(i+1)*8].gf_multiply_modular(e,modulus,n) ^ \
                                   block[(i+4)*8:(i+5)*8].gf_multiply_modular(b,modulus,n) ^ \
                                   block[(i+8)*8:(i+9)*8].gf_multiply_modular(d,modulus,n)
    for i in range(8,12):
        mixed_block[i*8:(i+1)*8] = block[(i-8)*8:(i-7)*8].gf_multiply_modular(d,modulus,n) ^ \
                                   block[(i-4)*8:(i-3)*8].gf_multiply_modular(nine,modulus,n) ^ \
                                   block[(i)*8:(i+1)*8].gf_multiply_modular(e,modulus,n) ^ \
                                   block[(i+4)*8:(i+5)*8].gf_multiply_modular(b,modulus,n)
    for i in range(12,16):
        mixed_block[i*8:(i+1)*8] = block[(i-12)*8:(i-11)*8].gf_multiply_modular(b,modulus,n) ^ \
                                   block[(i-8)*8:(i-7)*8].gf_multiply_modular(d,modulus,n) ^ \
                                   block[(i-4)*8:(i-3)*8].gf_multiply_modular(nine,modulus,n) ^ \
                                   block[(i)*8:(i+1)*8].gf_multiply_modular(e,modulus,n)
    return mixed_block

def encrypt(round_keys, sBox):
    plaintext = BitVector( filename = 'plaintext.txt' )
    if(os.path.exists('encrypted.txt')):
       os.remove('encrypted.txt')
    #open the file for writing
    fileout = open('encrypted.txt', 'a')
    while(plaintext.more_to_read):
        curr_block = plaintext.read_bits_from_file(128)
        if len(curr_block) < 128:
            curr_block.pad_from_right(128 - len(curr_block))
        curr_block = curr_block ^ round_keys[0]
        for i in range(10):
            substituted_block = sub_bytes(curr_block, sBox)
            shifted_block = row_shift(substituted_block)
            if not i == 9:
                mixed_block = col_mix(shifted_block)
            else:
                mixed_block = shifted_block
            curr_block = mixed_block ^ round_keys[i+1]
        curr_block.write_to_file(fileout)
    fileout.close()

def decrypt(round_keys, sBox):
    plaintext = BitVector( filename = 'encrypted.txt' )
    if(os.path.exists('decrypted.txt')):
       os.remove('decrypted.txt')
    #open the file for writing
    fileout = open('decrypted.txt', 'a')
    while(plaintext.more_to_read):
        curr_block = plaintext.read_bits_from_file(128)
        if len(curr_block) < 128:
            curr_block.pad_from_right(128 - len(curr_block))
        curr_block = curr_block ^ round_keys[10]
        for i in range(10):
            shifted_block = inv_row_shift(curr_block)
            substituted_block = sub_bytes(shifted_block, sBox)
            key_block = substituted_block ^ round_keys[10-i-1]
            if not i == 9:
                mixed_block = inv_col_mix(key_block)
            else:
                mixed_block = key_block
            curr_block = mixed_block
        curr_block.write_to_file(fileout)
    fileout.close()

if __name__ == '__main__':
    key = BitVector(textstring = "yayboilermakers!")
    [encryptionSBox, decryptSBox] = genTables()
    key_words = gen_key_schedule_128(key)
    round_keys = []
    round_key = BitVector(size = 0)
    counter = 0
    for i in range(44):
        round_key = round_key + key_words[i]
        counter += 1
        if counter == 4:
            counter = 0
            round_keys.append(round_key)
            round_key = BitVector(size=0)
    encrypt(round_keys, encryptionSBox)
    decrypt(round_keys, decryptSBox)
