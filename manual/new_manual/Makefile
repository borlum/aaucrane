TARGET=manual
CRANELIB_PATH = ../../libcrane

INC += -I/usr/local/lib
INC += -I../../libcrane

GCC=gcc -std=gnu11
LIBS=-lm -lpthread -lcomedi

all: $(TARGET)

$(TARGET): $(TARGET).c
	cd $(CRANELIB_PATH) && make
	$(GCC) $(TARGET).c $(INC) $(CRANELIB_PATH)/libcrane.a $(LIBS) -o $(TARGET)

ANGLE: $(TARGET).c
	cd $(CRANELIB_PATH) && make
	$(GCC) $(TARGET).c $(INC) $(CRANELIB_PATH)/libcrane.a $(LIBS) -DANGLE -o $(TARGET)

clean:
	rm xpos_controller
