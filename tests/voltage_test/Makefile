TARGET = x_voltage

INC += -I/usr/local/lib

GCC=gcc -std=gnu11
LIBS=-lm -lpthread -lcomedi

all: $(TARGET)

$(TARGET): $(TARGET).c
	cd $(LIBCRANE_PATH) && make
	$(GCC) $(TARGET).c $(INC) $(LIBS) -o $(TARGET)

clean:
	rm $(TARGET)
