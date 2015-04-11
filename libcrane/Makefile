GCC     = gcc -std=gnu11 -Wall -pedantic

INC     = -I/usr/local/lib
LIBS    = -lm -lcomedi
TESTING = -D TESTING

all: libcrane

libcrane: libcrane.c libcrane.h
	$(GCC) -c libcrane.c $(LIBS) $(INC)
	ar rcs libcrane.a libcrane.o

testing: libcrane.c libcrane.h tests/libcrane_tests.c
	$(GCC) $(TESTING) -c libcrane.c $(INC)
	$(GCC) $(TESTING) tests/libcrane_tests.c $(INC) libcrane.o -o testsuite
	ar rcs libcrane.a libcrane.o

run_tests:
	@./testsuite

clean:
	@rm libcrane.o
	@rm testsuite

.PHONY: run_tests clean
