CRANELIB_PATH = ../../cranelib

INC += -I/usr/local/lib
INC += -I../../cranelib

GCC=gcc -std=gnu11
LIBS=-lm -lpthread -lcomedi

all: y_step

y_step: y_step.c
	cd $(CRANELIB_PATH) && make
	$(GCC) y_step.c $(INC) $(CRANELIB_PATH)/cranelib.o $(LIBS) -o y_step

clean:
	rm y_step