TARGET=manual
CRANELIB_PATH = ../../libcrane

INC += -I/usr/local/lib
INC += -I../../libcrane
INC += -I../../acc/include/

GCC=gcc -std=gnu11
LIBS=-lm -lpthread -lcomedi

SOURCE += $(TARGET).c
SOURCE += ../../acc/src/compensator.c

all: $(TARGET)

$(TARGET): $(TARGET).c
	cd $(CRANELIB_PATH) && make
	$(GCC) $(SOURCE) $(INC) $(CRANELIB_PATH)/libcrane.a $(LIBS) -o $(TARGET)

ANGLE: $(TARGET).c
	cd $(CRANELIB_PATH) && make
	$(GCC) $(SOURCE) $(INC) $(CRANELIB_PATH)/libcrane.a $(LIBS) -DANGLE -o $(TARGET)

clean:
	rm $(TARGET)
