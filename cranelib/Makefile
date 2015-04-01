INC=-I/usr/local/lib
GCC=gcc -Wall -pedantic
LIBS=-lm -lcomedi
TESTING=-D TESTING

all: cranelib

cranelib: cranelib.c cranelib.h
	$(GCC) -c cranelib.c $(LIBS) $(INC)

testing: cranelib.c cranelib.h tests/cranelib_tests.c
	$(GCC) $(TESTING) -c cranelib.c $(INC)
	$(GCC) $(TESTING) tests/cranelib_tests.c $(INC) cranelib.o -o testsuite

run_tests:
	@./testsuite

clean:
	@rm cranelib.o
	@rm testsuite

.PHONY: run_tests clean