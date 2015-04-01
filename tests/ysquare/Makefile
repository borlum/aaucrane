CRANELIB_PATH = ../../cranelib

INC += -I/usr/local/lib
INC += -I../../cranelib

GCC=gcc -std=gnu11
LIBS=-lm -lpthread -lcomedi

all: y_square

y_square: y_square.c
	cd $(CRANELIB_PATH) && make
	$(GCC) y_square.c $(INC) $(CRANELIB_PATH)/cranelib.o $(LIBS) -o y_square

clean:
	rm y_square
