#ifndef _COMPENSATOR_H_
#define _COMPENSATOR_H

double angle_controller(double angle_pos, int aw);
double position_controller_x(double error);
double position_controller_y(double error);
double pid_get_controller_output();
void init_ramp(double x_ref);
int ramp_maker(double step);

#endif