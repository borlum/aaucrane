INC=-I/usr/realtime/bin -I/usr/realtime/include -I/usr/local/lib
GCC=gcc -std=gnu11
LIBS=-lm -lpthreads

all: x_step x_square

x_step: x_step.c
	gcc -std=gnu11 x_step.c -lm -lcomedi -lpthread $(INC) -o x_step

x_square: x_square.c
	$(GCC) x_square $(LIBS) $(INC) -o x_squre

clean:
	rm x_step
