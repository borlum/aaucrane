#ifndef _FILTER_H_
#define _FILTER_H

double angle_controller(double error);
double position_controller_x(double error);
double position_controller_y(double error);
double velocity_controller_x(double error);

#endif
