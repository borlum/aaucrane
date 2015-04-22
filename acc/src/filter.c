#include<stdio.h>

#include "../include/filter.h"
const static double TS = 0.001;

double error_sum = 0;
double old_error = 0;

double angle_controller(double error){
  const double k_p = 10;
  const double k_i = 0;
  const double k_d = 10;


  double out;

  out = k_p * error + k_d * (error - old_error) / TS;

  error_sum += error;
  old_error = error;

  printf("#### PD ####\n");
  printf("Error %lf\n", error);
  printf("Old error: %lf\n", old_error);
  printf("P: %lf\n", k_p * error);
  printf("D: %lf\n", k_d * (error - old_error) / TS);
  printf("PD: %lf\n", out);
  printf("############\n");

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
