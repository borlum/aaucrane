INC=-I/usr/realtime/bin -I/usr/realtime/include -I/usr/local/lib
GCC=gcc -std=gnu11
LIBS=-lm -lpthread -lcomedi

all: x_step

x_step: x_step.c
	$(GCC) x_step.c $(LIBS) $(INC) -o x_step

clean:
	rm x_step