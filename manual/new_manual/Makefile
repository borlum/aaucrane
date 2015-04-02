CRANELIB_PATH = ../../libcrane

INC += -I/usr/local/lib
INC += -I../../libcrane

GCC=gcc -std=gnu11
LIBS=-lm -lpthread -lcomedi

all: manual

manuel: manual.c
	cd $(CRANELIB_PATH) && make
	$(GCC) manual.c $(INC) $(CRANELIB_PATH)/libcrane.a $(LIBS) -o manual

clean:
	rm y_square
