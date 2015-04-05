TARGET=acc
CRANELIB_PATH = ../libcrane

INC += -I/usr/local/lib
INC += -I../libcrane

GCC=gcc -std=gnu11
LIBS=-lm -lpthread -lrt -lcomedi

SOURCE += $(TARGET).c
SOURCE += src/controller.c


all: $(TARGET)

$(TARGET): $(TARGET).c
	cd $(CRANELIB_PATH) && make
	$(GCC) $(SOURCE) $(INC) $(CRANELIB_PATH)/libcrane.a $(LIBS) -o $(TARGET)

test: $(TARGET).c
	$(GCC) $(SOURCE) -DTEST -I/usr/local/lin -lpthread -lrt -o $(TARGET)

clean:
	rm $(TARGET)
