#ifndef _FILTER_H_
#define _FILTER_H

double angle_controller(double error);
double position_controller_x(double error);
double position_controller_y(double error);
double velocity_controller_x(double error);
int ref_controller(double error, double* output, size_t out_len);
int ramp_maker(double step, double *ramp_arr);

#endif
