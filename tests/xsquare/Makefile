INC=-I/usr/realtime/bin -I/usr/realtime/include -I/usr/local/lib
GCC=gcc -std=gnu11
LIBS=-lm -lpthread -lcomedi

all: x_square

x_square: x_square.c
	$(GCC) x_square.c $(LIBS) $(INC) -o x_square

clean:
	rm x_square