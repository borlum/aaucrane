CRANELIB_PATH = ../../cranelib

INC += -I/usr/realtime/bin
INC += -I/usr/realtime/include
INC += -I/usr/local/lib
INC += -I../../cranelib

GCC=gcc -std=gnu11
LIBS=-lm -lpthread -lcomedi

all: x_step

x_step: x_step.c
	cd $(CRANELIB_PATH) && make
	$(GCC) x_step.c $(LIBS) $(INC) $(CRANELIB_PATH)/cranelib.o -o x_step

clean:
	rm x_step