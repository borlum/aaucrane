CRANELIB_PATH = ../../libcrane

INC += -I/usr/local/lib
INC += -I../../libcrane

GCC=gcc -std=gnu11
LIBS=-lm -lpthread -lcomedi

all: manuel

manuel: manuel.c
	cd $(CRANELIB_PATH) && make
	$(GCC) manuel.c $(INC) $(CRANELIB_PATH)/libcrane.a $(LIBS) -o manuel

clean:
	rm y_square
