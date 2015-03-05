export LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib

all: x-step

x-step: x-step.c
	gcc -std=gnu11 x-step.c -lm -lcomedi -o x-step

test: x-step.c
	gcc -std=gnu11 x-step.c -o x-step

clean:
	rm x-step