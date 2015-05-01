TARGET=monitor
CRANELIB_PATH = ../../libcrane

INC += -I/usr/local/lib
INC += -I$(CRANELIB_PATH)
INC += -I../../acc/include/

GCC=gcc -std=gnu11
LIBS=-lm -lpthread -lrt -lcomedi

SOURCE += $(TARGET).c

all: $(TARGET)

$(TARGET): $(TARGET).c
	cd $(CRANELIB_PATH) && make
	$(GCC) $(SOURCE) $(INC) $(CRANELIB_PATH)/libcrane.a $(LIBS) -o $(TARGET)

clean:
	rm $(TARGET)