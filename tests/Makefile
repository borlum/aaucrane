all: x_step

x-step: x_step.c
	gcc -std=gnu11 x_step.c -lm -lcomedi -lpthread -I /usr/realtime/include -I /usr/realtime/bin -I /usr/local/lib -o x_step

clean:
	rm x_step