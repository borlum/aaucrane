#include "../include/filter.h"


double angle_controller(double error){
  double k_p = 5;
  return error * k_p;
}

double position_controller_x(double error){
  double k_p = 100;
  return error * k_p;
}

double position_controller_y(double error){
  double k_p = 49.3;
  return error * k_p;
}

double velocity_controller_x(double error){
  double k_p = 1;
  return k_p * error;
}