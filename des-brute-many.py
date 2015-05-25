#!/usr/bin/env python
from __future__ import print_function
from subprocess import Popen
import sys

if __name__ == '__main__':
    plaintext  = '1234123412341234'
    ciphertext = '57475a4f836b31d5'
    keybase    = '5678567856780000'
    bits_from  = 16
    bits_to    = 40

    for bits in range(bits_from, bits_to + 1):
        keyfrom = '%016x' % ((int(keybase, 16) >> bits) << bits)
        keyto =   '%016x' % ((int(keybase, 16) >> bits) + 1 << bits)
        cmd = ['qsub',
               '-v', 'PLAINTEXT="%s",CIPHERTEXT="%s",KEYFROM="%s",KEYTO="%s"' % (plaintext, ciphertext, keyfrom, keyto),
               #'-o', 'out/result_%s_bits.txt' % bits,
               'des-brute.pbs']
        print(' '.join(cmd))
        proc = Popen(cmd, stdout=sys.stdout, stderr=sys.stderr)
        proc.wait()
