#include "../include/filter.h"

double angle_controller(double error){
  double k_p = -2;
  return error * k_p;
}

double position_controller_x(double error){
  double k_p = 26.3;
  return error * k_p;
}

double position_controller_y(double error){
  double k_p = 80;
  return error * k_p;
}

