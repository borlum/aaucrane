#!/usr/bin/env python3

import sys
import subprocess
import random

if __name__ == '__main__':
    random.seed(42)
    try:
        acc = subprocess.Popen("stdbuf -oL sudo ./acc", shell = True, stdout = subprocess.PIPE, stdin = subprocess.PIPE)
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

            input("Hit enter")
            msg = "{0},{1} {2},{3}\n".format(source_col, source_row, dest_col, dest_row)
            acc.communicate(input=bytes(msg, 'ascii'))[0]
            
            source_col = dest_col
            dest_col = random.randint(1, 22)

        acc.communicate()
