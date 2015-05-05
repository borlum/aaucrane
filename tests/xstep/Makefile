TARGET = x_step
LIBCRANE_PATH = ../../libcrane

INC += -I/usr/local/lib
INC += -I../../libcrane

RTAI_LIB += -I/usr/realtime/include/
RTAI_LIB += -I/usr/realtime/bin/

GCC=gcc -std=gnu11
LIBS=-lm -lpthread -lcomedi

SOURCE += $(TARGET).c
SOURCE += src/controller.c
SOURCE += src/filter.c


all: $(TARGET)

$(TARGET): $(TARGET).c
	cd $(LIBCRANE_PATH) && make
	$(GCC) $(TARGET).c -DRTAI $(RTAI_LIB) $(INC) $(LIBCRANE_PATH)/libcrane.o $(LIBS) -o $(TARGET)

clean:
	rm $(TARGET)
