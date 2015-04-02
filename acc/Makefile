TARGET=acc
CRANELIB_PATH = ../libcrane

INC += -I/usr/local/lib
INC += -I../libcrane

GCC=gcc -std=gnu11
LIBS=-lm -lpthread -lrt -lcomedi

all: $(TARGET)

$(TARGET): $(TARGET).c
	cd $(CRANELIB_PATH) && make
	$(GCC) $(TARGET).c $(INC) $(CRANELIB_PATH)/libcrane.a $(LIBS) -o $(TARGET)

test: $(TARGET).c
	$(GCC) $(TARGET).c -DTEST -I/usr/local/lin -lpthread -lrt -o $(TARGET)

clean:
	rm $(TARGET)
