#!/usr/bin/env python3

import sys
import subprocess
import random

if __name__ == '__main__':
    random.seed(42)
    try:
        acc = subprocess.Popen('sudo ./acc', shell = True, stdout = subprocess.PIPE)
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
                print(line)
                if(line == 'Enter a crane command <row,col row,col>:\n'):
                    break

            input("Hit enter")
            acc.stdout.write("{0},{1} {2},{3}\n", source_col, source_row, dest_col, dest_row)
            
            source_col = dest_col
            dest_col = random.randint(1, 22)
