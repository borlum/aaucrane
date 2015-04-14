TARGET = y_step
LIBCRANE_PATH = ../../libcrane

INC += -I/usr/local/lib
INC += -I../../libcrane

GCC=gcc -std=gnu11
LIBS=-lm -lpthread -lcomedi

all: $(TARGET)

$(TARGET): $(TARGET).c
	cd $(LIBCRANE_PATH) && make
	$(GCC) $(TARGET).c $(INC) $(LIBCRANE_PATH)/libcrane.o $(LIBS) -o $(TARGET)

clean:
	rm $(TARGET)