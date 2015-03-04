# Comedi related
export LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
# RTAI related
export PATH=$PATH:/usr/realtime/include
export PATH=$PATH:/usr/realtime/bin

all: example

example: rtai_example.c
	gcc -std=gnu11 rtai_example.c -lm -lcomedi -o example

clean:
	rm example