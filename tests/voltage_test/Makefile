TARGET = x_voltage

INC += -I/usr/local/lib

GCC=gcc -std=gnu11
LIBS=-lm -lpthread -lcomedi

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(GCC) $(TARGET).c $(INC) $(LIBS) -o $(TARGET)

clean:
	rm $(TARGET)
