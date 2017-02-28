#!/usr/bin/python3.4

import sys
from PrimeGenerator import *
from BitVector import *
import os

def get_modular_exp(A, B, n):
    result = 1
    while B>0:
        if B & 1:
            result = (result * A) % n
        B = B >> 1
        A = (A * A) % n
    return result

def encrypt(message_file, output_file, e_bv, n_bv):
    bv = BitVector( filename = message_file )
    #delete any existing file named "encrypted.txt"
    if(os.path.exists(output_file)):
       os.remove(output_file)
    #open the file for writing
    FILEOUT = open(output_file, 'ab')
    while (bv.more_to_read):
        #take 64 bits
        bitvec = bv.read_bits_from_file(128)
        e_msg = get_modular_exp(bitvec.intValue(), e_bv.intValue(), n_bv.intValue())
        output = BitVector(intVal = e_msg, size = 256)
        output.write_to_file(FILEOUT)
    FILEOUT.close()

def decrypt(encrypted_file, decrypted_file, d_bv, p_bv, q_bv):
    bv = BitVector( filename = encrypted_file )
    #delete any existing file named "encrypted.txt"
    if(os.path.exists(decrypted_file)):
       os.remove(decrypted_file)
    #open the file for writing
    FILEOUT = open(decrypted_file, 'ab')
    while (bv.more_to_read):
        #take 64 bits
        bitvec = bv.read_bits_from_file(256)
        #Chinese Remainder Theorem
        v_p = get_modular_exp(bitvec.intValue(), d_bv.intValue(),p_bv.intValue())
        v_q = get_modular_exp(bitvec.intValue(), d_bv.intValue(),q_bv.intValue())
        x_p = q_bv.intValue() * q_bv.multiplicative_inverse(p_bv).intValue()
        x_q = p_bv.intValue() * p_bv.multiplicative_inverse(q_bv).intValue()
        d_msg = (v_p * x_p + v_q * x_q) % (p_bv.intValue() * q_bv.intValue())
        output = BitVector(intVal = d_msg, size = 256)
        output.write_to_file(FILEOUT)
    FILEOUT.close()

if __name__=="__main__":
    if(sys.argv[1] == "-e"):
        e = 65537
        e_bv = BitVector(intVal = e)
        one_bv = BitVector(intVal = 1)
        generator = PrimeGenerator(bits = 128)
        found = False
        while(not found):
            found = True
            p = generator.findPrime()
            q = generator.findPrime()
            p_bv = BitVector(intVal = p)
            q_bv = BitVector(intVal = q)
            p_bv[0] = 1
            q_bv[0] = 1
            if(p == q):
                found = False
            if BitVector(intVal=(p-1)).gcd(e_bv).intValue() != 1 or BitVector(intVal=(q-1)).gcd(e_bv).intValue() != 1:
                found = False

        if(os.path.exists("p.txt")):
            os.remove("p.txt")
        FILEOUTp = open("p.txt", 'ab')
        p_bv.write_to_file(FILEOUTp)
        FILEOUTp.close()
        if(os.path.exists("q.txt")):
            os.remove("q.txt")
        FILEOUTq = open("q.txt", 'ab')
        q_bv.write_to_file(FILEOUTq)
        FILEOUTq.close()
        d_bv = e_bv.multiplicative_inverse(BitVector(intVal=(p-1)*(q-1)))
        n_bv = BitVector(intVal = p*q)
        encrypt(sys.argv[2], sys.argv[3], e_bv, n_bv)
    elif(sys.argv[1] == "-d"):
        e = 65537
        e_bv = BitVector(intVal = e)
        bv_temp = BitVector(filename = "p.txt")
        p_bv = bv_temp.read_bits_from_file(128)
        bv_temp = BitVector(filename = "q.txt")
        q_bv = bv_temp.read_bits_from_file(128)
        p = p_bv.intValue()
        q = q_bv.intValue()
        d_bv = e_bv.multiplicative_inverse(BitVector(intVal=(p-1)*(q-1)))

        decrypt(sys.argv[2], sys.argv[3], d_bv, p_bv, q_bv)
    else:
        print("Select either -e or -d")

