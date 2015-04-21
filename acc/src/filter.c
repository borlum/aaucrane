#include "../include/filter.h"
double error_sum = 0, error_old = 0;

double angle_controller(double error){
  double k_p = 45.8;
  double k_i = 1; k_d = 10;
   double out;

  out = error * (k_p + k_d * (error-error_old)*1/1000 + k_i * error_sum);
  error_old = error;
  error_sum += error;
  return out;
}

double position_controller_x(double error){
  double k_p = 60;
  return error * k_p;
}

double position_controller_y(double error){
  double k_p = 49.3;
  return error * k_p;
}

double velocity_controller_x(double error){
  double k_p = 50;
  return k_p * error;
}
