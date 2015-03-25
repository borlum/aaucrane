# Comedi related
export LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
# RTAI related
export PATH=$PATH:/usr/realtime/include
export PATH=$PATH:/usr/realtime/bin

all: example

example: no_reg.c
	gcc -std=gnu11 no_reg.c -lm -lcomedi -o no_reg

clean:
	rm example
