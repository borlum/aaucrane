#ifndef _LIBCRANE_H_
#define _LIBCRANE_H_

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <termios.h>
#include <stdlib.h>
#include <string.h>

#ifndef TESTING
#include <comedilib.h>
#endif

#define NANO_SEC 1000000000

/*CRANE CONFIGURATION*/
#define AIN_SUBDEV 0
#define AOUT_SUBDEV 1
#define DIO_SUBDEV 2

#define DEVICE "/dev/comedi0"

#define SERIAL_PORT "/dev/ttyACM0"

#define CHAN_MAGNET_OUT 7
#define CHAN_MAGNET_BTN 3

#define CHAN_CTRL_BTN 2

#define CHAN_XMOTOR_OUT 0
#define CHAN_YMOTOR_OUT 1

#define CHAN_ANGLE_IN 12
#define CHAN_ANGLE_OLD_IN 0

#define CHAN_XPOS_IN 13
#define CHAN_YPOS_IN 2

#define CHAN_XVEL_IN 11
#define CHAN_YVEL_IN 4

#define CHAN_XIN_IN 9
#define CHAN_YIN_IN 10

#define CHAN_CTRLPAD_X_IN 14
#define CHAN_CTRLPAD_Y_IN 15

double libcrane_truncate(double sutff_oreo);

int initialize_crane();

int run_motorx(double voltage);
int run_motory(double voltage);
int run_motor(double voltage, int axis);

double get_angle();
double get_angle_raw();

double get_old_angle_raw();

double get_xpos();
double get_xpos_raw();

double get_ypos();
double get_ypos_raw();

double get_motorx_velocity();
double get_motorx_velocity_raw();

double get_x_velocity();

double get_motory_velocity();
double get_motory_velocity_raw();

double get_motorx_voltage();
double get_motory_voltage();

double get_ctrlpad_x();
double get_ctrlpad_y();

int get_ctrlpad_magnet_switch();
int get_ctrlpad_ctrl_switch();

void enable_magnet();
void disable_magnet();

double get_sensor_raw(int channel);

int get_sensor_pixel();

unsigned long get_time_micros();

#endif /* _LIBCRANE_H_ */