# Comedi related
export LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
# RTAI related
export PATH=$PATH:/usr/realtime/include
export PATH=$PATH:/usr/realtime/bin

all: example

example: rtaiExample.c
	gcc -std=gnu11 rtaiExample.c -lm -lcomedi -o example

clean:
	rm example