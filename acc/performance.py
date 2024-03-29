#!/usr/bin/env python3

import sys
import subprocess
import random
import time

if __name__ == '__main__':
    random.seed(42)
    try:
        acc = subprocess.Popen("stdbuf -oL ./a.out", shell = True, stdout = subprocess.PIPE, stdin = subprocess.PIPE)
    except:
        print("....")
    else:
        source_row = 0
        source_col = 1

        dest_row = 0
        dest_col = 22
    
        for i in range(100):
            while(True):
                line = acc.stdout.readline()
                if line != b'':
                    print(line)
                if(line == b'Enter a crane command <row,col row,col>:\n'):
                    break
                
            msg = "{0},{1} {2},{3}\n".format(source_col, source_row, dest_col, dest_row)
            print(bytes(msg, 'ascii'))
            resp = acc.communicate(input=bytes(msg, 'ascii'), timeout=30)[0]
            #print(acc.stdin.write(bytes(msg, 'ascii')))
            #acc.call(msg)
            print(resp)
            source_col = dest_col
            dest_col = random.randint(1, 22)

        #acc.communicate()
