# Comedi related
export LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib

all: example

example: no_reg_magnet_log_data.c
	gcc -std=gnu11 no_reg_magnet_log_data.c -lm -lcomedi -o no_reg_magnet_log_data

clean:
	rm no_reg_magnet_log_data
