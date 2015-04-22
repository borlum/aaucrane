#include<stdio.h>

#include "../include/filter.h"
const static double TS = 0.001;

double error_sum = 0;
double old_error = 0;

double angle_controller(double error){
  const double k_p = 50;
  const double k_i = 0;
  const double k_d = .1;


  double p = 0, i = 0, d = 0, out = 0;

  p = k_p * error;
  i = 0;
  d = (k_d * (error - old_error)) / TS;
  
  
  out = p + d;

  printf("#### PD ####\n");
  printf("Error %lf\n", error);
  printf("Old error: %lf\n", old_error);
  printf("P: %lf\n", p);
  printf("D: %lf\n", d);
  printf("PD: %lf\n", out);
  printf("############\n");

  old_error = error;
  
  return out;
}

double position_controller_x(double error){
  double k_p = 25;
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
