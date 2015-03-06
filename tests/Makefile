INC=-I/usr/realtime/bin -I/usr/realtime/include -I/usr/local/lib

all: x_step

x_step: x_step.c
	gcc -std=gnu11 x_step.c -lm -lcomedi -lpthread $(INC) -o x_step

clean:
	rm x_step