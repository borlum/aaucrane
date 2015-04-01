#include <stdio.h>
#include <time.h>

#ifndef TESTING
#include <comedilib.h>
#endif

#define NANO_SEC 1000000000

/*CRANE CONFIGURATION*/
#define AIN_SUBDEV 0
#define AOUT_SUBDEV 1
#define DIO_SUBDEV 2
#define DEVICE "/dev/comedi0"

#define CHAN_MAGNET_OUT 7
#define CHAN_MAGNET_BTN 3

#define CHAN_XMOTOR_OUT 0
#define CHAN_YMOTOR_OUT 1

#define CHAN_ANGLE_IN 12

#define CHAN_XPOS_IN 13
#define CHAN_YPOS_IN 2

#define CHAN_XVEL_IN 11
#define CHAN_YVEL_IN 4

#define CHAN_XIN_IN 9
#define CHAN_YIN_IN 10

int initialize_crane();

int run_motorx(int voltage);
int run_motory(int voltage);
int run_motor(int voltage, int axis);

double get_angle();
double get_angle_raw();

double get_xpos();
double get_xpos_raw();

double get_ypos();
double get_ypos_raw();

double get_motorx_velocity();
double get_motorx_velocity_raw();

double get_motory_velocity();
double get_motory_velocity_raw();

double get_motorx_voltage();
double get_motory_voltage();

double get_sensor_raw(int channel);

unsigned long get_time_millis();