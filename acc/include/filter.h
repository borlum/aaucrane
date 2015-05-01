#ifndef _FILTER_H_
#define _FILTER_H

double angle_controller(double angle_pos, int aw);
double position_controller_x(double error);
double position_controller_y(double error);
double velocity_controller_x(double error);
int ramp_maker(double step, double *ramp_arr);
double angle_controller2(double angle_pos);
double angle_controller3(double error);
double position_controller_x2(double error);
double position_controller_x3(double error);

#endif
