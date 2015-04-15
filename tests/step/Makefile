TARGET=step
CRANELIB_PATH = ../../libcrane

INC += -I/usr/local/lib
INC += -I$(CRANELIB_PATH)
INC += -I../../acc/include/

GCC=gcc -std=gnu11
LIBS=-lm -lpthread -lrt -lcomedi

SOURCE += $(TARGET).c
SOURCE += ../../acc/src/controller.c
SOURCE += ../../acc/src/filter.c


all: $(TARGET)

$(TARGET): $(TARGET).c
	cd $(CRANELIB_PATH) && make
	$(GCC) $(SOURCE) $(INC) $(CRANELIB_PATH)/libcrane.a $(LIBS) -o $(TARGET)

test: $(TARGET).c
	$(GCC) $(SOURCE) -DTEST -I/usr/local/lib -Iinclude/ -lpthread -lrt -o $(TARGET)

clean:
	rm $(TARGET)