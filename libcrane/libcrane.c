#include "libcrane.h"
#include <math.h>
#ifndef TESTING
comedi_t *NI_card;
#else
int *NI_card;
#endif

static const double MAX_MOTOR_OUTPUT = 12.5;
static const double MIN_MOTOR_OUTPUT = 5.5;
static double ZERO_ANGLE_RAD;

/**
 * Open comedi driver for interfacing w. crane
 * @return Return 0 if failed to init, 1 if success
 */
int initialize_crane()
{
    NI_card = NULL;

    #ifndef TESTING
    NI_card = comedi_open(DEVICE);
    /*Global config*/
    comedi_set_global_oor_behavior(COMEDI_OOR_NUMBER);
    comedi_dio_config(NI_card, DIO_SUBDEV, CHAN_MAGNET_OUT, COMEDI_OUTPUT);
    comedi_dio_config(NI_card, DIO_SUBDEV, CHAN_MAGNET_BTN, COMEDI_INPUT);
    #endif

    if (NI_card == NULL) {
        return 0;
    }

    // Find zero degree angle voltage
    ZERO_ANGLE_RAD = 0.7367*get_angle_raw() - 1.3211;
//    printf("%lf\n", get_angle());

    return 1;
}

/**
 * Runs motor controlling x-axis
 * @param   voltage Voltage supplied to the motor
 * @return  Input value for comedi driver, or -1 if fail
 */
int run_motorx(int voltage)
{
    return run_motor(-voltage, 0); /* Change X motor direction */
}

/**
 * Runs motor controlling y-axis
 * @param   voltage Voltage supplied to the motor
 * @return Input value for comedi driver, or -1 if fail
 */
int run_motory(int voltage)
{
    return run_motor(voltage, 1);
}

/**
 * Runs motor controlling axis given by parameter
 * @param    voltage Voltage supplied to the motor
 * @param    axis The axis to control (0 = X, 1 = Y)
 * @return Input value for comedi driver, or -1 if fail
 */
int run_motor(int voltage, int axis)
{
    /* int old_val, new_val; */
    /* int old_range, new_range, old_max, old_min, new_max, new_min; */

  int output;
  
    if (voltage > MAX_MOTOR_OUTPUT) {
        voltage = MAX_MOTOR_OUTPUT;
    }

    if (voltage < -MAX_MOTOR_OUTPUT) {
        voltage = -MAX_MOTOR_OUTPUT;
    }

    if (voltage > -MIN_MOTOR_OUTPUT && voltage < 0) {
      voltage = -MIN_MOTOR_OUTPUT;
    }

    if (voltage < MIN_MOTOR_OUTPUT && voltage > 0) {
      voltage = MIN_MOTOR_OUTPUT;
    }

    output = (int) (157.5 * voltage + 2024);

    /* old_val = voltage; */

    /* old_max =  MAX_MOTOR_OUTPUT; new_max  = 4400; */
    /* old_min = -MAX_MOTOR_OUTPUT; new_min  =    0; */

    /* old_range = old_max - old_min; */
    /* new_range = new_max - new_min; */

    /* /\* SEE: */
    /* http://stackoverflow.com/questions/929103/convert-a-number-range-to-another-range-maintaining-ratio */
    /*  *\/ */
    /* new_val = (((old_val - old_min) * new_range) / old_range) + new_min; */

    #ifndef TESTING

    if (axis == 0) {
        axis = CHAN_XMOTOR_OUT;
    } else if (axis == 1) {
        axis = CHAN_YMOTOR_OUT;
    } else {
        /*WRONG AXIS PARAMETER!*/
        return -1;
    }

    if (comedi_data_write(NI_card, AOUT_SUBDEV, axis, 0, AREF_GROUND, output) == -1) {
        return -1;
    }
    #endif

    return output;
}

/**
 * Samples the current payload angle
 * @return Current angle in radians
 */
double get_angle()
{
    return (0.7367*get_angle_raw() - 1.3211) - ZERO_ANGLE_RAD;
}

/**
 * Samples the current payload angle
 * @return Current angle in volts
 */
double get_angle_raw()
{
    return get_sensor_raw(CHAN_ANGLE_IN);
}

/**
 * Samples the current payload angle using potmeter
 * @return Current angle in volts
 */
double get_old_angle_raw()
{
    return get_sensor_raw(CHAN_ANGLE_OLD_IN);
}

/**
 * Samples the current x position
 * @return Current position in metres
 */
double get_xpos()
{
    return  (-0.4981)*get_xpos_raw() + 4.7931;
    /*return -((get_xpos_raw() * 0.5) - 0.8) + 4.0;*/
}

/**
 * Samples the current x position (trolley)
 * @return Current position in volts
 */
double get_xpos_raw()
{
    return get_sensor_raw(CHAN_XPOS_IN);
}

/**
 * Samples current y position (magnet)
 * @return Current position in metres
 */
double get_ypos()
{
    return (-0.1536)*get_ypos_raw() + 1.2106;
    /*return ((get_ypos_raw() * - 0.15) + 1.35) - 0.2;*/
}

/**
 * Samples current y position (magnet)
 * @return Current position in volts
 */
double get_ypos_raw()
{
    return get_sensor_raw(CHAN_YPOS_IN);
}

/**
 * Samples current motor(x) velocity (shaft)
 * @return Current velocity in rad/s
 */
double get_motorx_velocity()
{
    return (get_motorx_velocity_raw() * 34.8) - 4.0;
}

/**
 * Samples current motor(x) velocity (shaft)
 * @return Current velocity in volts
 */
double get_motorx_velocity_raw()
{
    return get_sensor_raw(CHAN_XVEL_IN);
}

/**
 * Samples current motor(y) velocity (shaft)
 * @return Current velocity in rad/s
 */
double get_motory_velocity()
{
    /*TODO: THIS IS WRONG!!!*/
    return (get_motory_velocity_raw() * 34.8) - 4.0;
}

/**
 * Samples current motor(y) velocity (shaft)
 * @return Current velocity in volts
 */
double get_motory_velocity_raw()
{
    return get_sensor_raw(CHAN_YVEL_IN);
}

/**
 * Samples input voltage supplied to motor(x)
 * @return Voltage supplied to motor
 */
double get_motorx_voltage()
{
    return get_sensor_raw(CHAN_XIN_IN) * 2 * 0.984 - 0.2230;
}

/**
 * Samples input voltage supplied to motor(y)
 * @return Voltage supplied to motor
 */
double get_motory_voltage()
{
    return get_sensor_raw(CHAN_YIN_IN) * 2 * 0.984 - 0.2230;
}

/**
 * Samples current x-axis voltage of control pad
 * @return Control pad x-axis voltage
 */
double get_ctrlpad_x()
{
  double raw_val;

  double old_val, new_val;
  double old_range, new_range, old_max, old_min, new_max, new_min;

  raw_val = get_sensor_raw(CHAN_CTRLPAD_X_IN);

  old_val = raw_val - 0.9;

  old_max =  10; new_max   =  MAX_MOTOR_OUTPUT;
  old_min = 0;   new_min   = -MAX_MOTOR_OUTPUT;

  old_range = old_max - old_min;
  new_range = new_max - new_min;

  new_val = -( (((old_val - old_min) * new_range) / old_range) + new_min );

  return new_val - 0.5; /* The sensor is biased, so we need to pull it down a bit */
}
/**
 * Samples current y-axis voltage of control pad
 * @return Control pad y-axis voltage
 */
double get_ctrlpad_y()
{
  double raw_val;

  double old_val, new_val;
  double old_range, new_range, old_max, old_min, new_max, new_min;

  raw_val = get_sensor_raw(CHAN_CTRLPAD_Y_IN);

  old_val = raw_val;

  old_max =  10; new_max   =  MAX_MOTOR_OUTPUT;
  old_min = 0;   new_min   = -MAX_MOTOR_OUTPUT;

  old_range = old_max - old_min;
  new_range = new_max - new_min;

  new_val = (((old_val - old_min) * new_range) / old_range) + new_min;

  return new_val;
}

/**
 * Gets current state of magnet switch on control pad
 * @return 1 = ON, 0 = OFF
 */
int get_ctrlpad_magnet_switch()
{
    #ifndef TESTING
    lsampl_t in;
    comedi_dio_read(NI_card, DIO_SUBDEV, CHAN_MAGNET_BTN, &in);
    #else
    int in = 1;
    #endif

    return (int)in;
}

/**
 * Turn on magnet
 */
void enable_magnet()
{
    #ifndef TESTING
    comedi_dio_write(NI_card, DIO_SUBDEV, CHAN_MAGNET_OUT, 1);
    #endif
}

/**
 * Turn off magnet
 */
void disable_magnet()
{
    #ifndef TESTING
    comedi_dio_write(NI_card, DIO_SUBDEV, CHAN_MAGNET_OUT, 0);
    #endif
}

/**
 * Get an arbitrary raw sensor value specified by channel
 * @param    channel The channel to read from
 * @return Current sensor output in volts
 */
double get_sensor_raw(int channel)
{
    double physical_value = -1;

    #ifndef TESTING
    lsampl_t data, max_data;
    comedi_range *range_info;

    comedi_data_read(NI_card, AIN_SUBDEV, channel, 0, AREF_GROUND, &data);

    range_info     = comedi_get_range(NI_card, AIN_SUBDEV, channel, 0);
    max_data       = comedi_get_maxdata(NI_card, AIN_SUBDEV, channel);
    physical_value = comedi_to_phys(data, range_info, max_data);

    return physical_value;
    #endif

    return physical_value;
}

/**
 * Get current time in microseconds
 * @return Current time in us
 */
unsigned long get_time_micros()
{
    struct timespec tm;
    clock_gettime(CLOCK_REALTIME, &tm);
    return (tm.tv_nsec + (unsigned long long)tm.tv_sec * NANO_SEC) / 1000;
}
