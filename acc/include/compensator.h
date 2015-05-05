#ifndef _COMPENSATOR_H_
#define _COMPENSATOR_H

#define SAMPLE_TIME_NS 1E6
#define SAMPLE_TIME_S SAMPLE_TIME_NS * 10E-9 

double angle_controller(double angle_pos, int aw);
double position_controller_x(double error);
double position_controller_y(double error);
double pid_get_controller_output();
void init_ramp(double x_ref);
int ramp_maker(double step);

#endif
