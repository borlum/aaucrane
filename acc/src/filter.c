#include <stdlib.h>

#include "../include/filter.h"
const static double TS = 1/1000;

double angle_controller(double error){
  const static double k_p = 45.8;
  const static double k_i = 1;
  const static double k_d = 10;

  static error_sum = 0;
  static old_error = 0;
  
  double out;

  out = k_p * error + k_i * error_sum + k_d * (error - old_error) * TS;

  error_sum += error * TS;
  old_error = error;

  printf("PID OUT: %lf\n", out);
  
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
