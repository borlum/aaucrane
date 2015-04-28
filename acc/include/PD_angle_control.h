#ifndef _PD_ANGLE_CONTROLLER_
#define _PD_ANGLE_CONTROLLER_

void pd_init_controller(double x_ref);
double pd_get_controller_output();
double pid_get_controller_output();
double ld_get_controller_output(double x_ref);

#endif
