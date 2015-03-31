#include "cranelib.h"

#ifndef TESTING
comedi_t *NI_card;
#else
int *NI_card;
#endif

/**
 * Open comedi driver for interfacing w. crane
 * @return Return 0 if failed to init, 1 if success
 */
int initialize_crane()
{   
    NI_card = NULL;

    #ifndef TESTING
    NI_card = comedi_open(DEVICE);
    #endif

    if (NI_card == NULL) {
        return 0;
    }

    return 1;
}

/**
 * Runs motor controlling x-axis
 * @param   voltage Voltage supplied to the motor
 * @return  Input value for comedi driver, or -1 if fail
 */
int run_motorx(int voltage)
{
    return run_motor(voltage, 0);
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
    int old_val, new_val;
    int old_range, new_range, old_max, old_min, new_max, new_min;

    if (voltage > 14) {
        voltage = 14;
    }

    if (voltage < -14) {
        voltage = -14;
    }

    old_val = voltage;
    
    old_max =  14; new_max  = 4096;
    old_min = -14; new_min  =    0;

    old_range = old_max - old_min;
    new_range = new_max - new_min;

    new_val = (((old_val - old_min) * new_range) / old_range) + new_min;

    #ifndef TESTING

    if (axis == 0) {
        axis = CHAN_XMOTOR_OUT;
    } else if (axis == 1) {
        axis = CHAN_YMOTOR_OUT;
    } else {
        /*WRONG AXIS PARAMETER!*/
        return -1;
    }

    if (comedi_data_write(NI_card, ANALOG_OUT_SUBDEV, axis, 0, AREF_GROUND, new_val) == -1) {
        return -1;
    }
    #endif

    return new_val;
}

/**
 * Samples and evaluates the current payload angle
 * @return Current angle in radians
 */
double get_angle()
{
    return 0.0;
}

/**
 * Samples and evaluates the current payload angle
 * @return Current angle in volts
 */
double get_angle_raw()
{
    return 0.0;
}