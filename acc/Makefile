TARGET = acc
GCC = gcc -std=gnu11

CRANELIB_PATH = ../libcrane

INC += -I/usr/local/lib
INC += -I../libcrane
INC += -Iinclude/

LIBS      = -lm -lpthread -lrt -lcomedi
LIBS_TEST = -lm -lpthread -lrt

SOURCE += $(TARGET).c
SOURCE += src/controller.c
SOURCE += src/filter.c
SOURCE += src/stack.c

TESTING = -D TESTING

all: $(TARGET)

$(TARGET): $(TARGET).c
	cd $(CRANELIB_PATH) && make
	$(GCC) $(SOURCE) $(INC) $(CRANELIB_PATH)/libcrane.a $(LIBS) -o $(TARGET)

test: $(TARGET).c
	cd $(CRANELIB_PATH) && make testing
	$(GCC) $(SOURCE) $(INC) $(CRANELIB_PATH)/libcrane.a $(LIBS_TEST) $(TESTING) -o $(TARGET)

clean:
	rm $(TARGET)
	cd $(CRANELIB_PATH) && make clean