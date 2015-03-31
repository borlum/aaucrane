#include <stdio.h>

#ifndef TESTING
#include <comedilib.h>
#endif

/*CRANE CONFIGURATION*/
#define ANALOG_IN_SUBDEV 0
#define ANALOG_OUT_SUBDEV 1
#define DIGITIAL_IO_SUBDEV 2
#define DEVICE "/dev/comedi0"

#define CHAN_MAGNET_OUT 7

#define CHAN_XMOTOR_OUT 0
#define CHAN_YMOTOR_OUT 1

int initialize_crane();

int run_motorx(int voltage);
int run_motory(int voltage);
int run_motor(int voltage, int axis);

double get_angle();
double get_angle_raw();