#include "../include/filter.h"

double x[3] = { 0 };
double y[3] = { 0 };
int n;
int x_old;

double angle_controller(double error){
  double k_p = 1;
  return error * k_p;
}

double position_controller_x(double error){
  double k_p = 100;
  double k_i = 0.5;
  x_old += error;
  return error * k_p + x_old*k_i;
}

double position_controller_y(double error){
  double k_p = 49.3;
  return error * k_p;
}

double velocity_controller_x(double error){
  double k_p = 1000;
  return error * k_p;
}

void set_ramp_ref(double x_ref){
  for(int i = 0; i < 3; i++)
    x[i] = x_ref;
}

double get_ramp_out(){
  y[n % 3] = 6.247E-08 * x[n % 3] + 1.249E-07 * x[(n & 3)-1] +  6.247E-08 * x[(n % 3)-2] - 2 * y[(n % 3)-1] + y[(n % 3)-2];
  n++;
  return y[n % 3];

}
