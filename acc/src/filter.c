#include<stdio.h>

#include "../include/filter.h"
const static double TS = 0.001;

double error_sum = 0;
double old_error = 0; 

double angle_controller(double error){
  const double k_p = 45.8;
  const double k_i = 1;
  const double k_d = 10;

  
  double out;

  out = k_p * error + k_i * error_sum * TS + k_d * (error - old_error) / TS;

  error_sum += error;
  old_error = error;

  printf("#### PID ####\n");
  printf("Error %lf\n", error);
  printf("Error sum %lf\n", error_sum);
  printf("Old error: %lf\n", old_error);
  printf("P: %lf\n", k_p * error);
  printf("I: %lf\n", k_i * error_sum * TS);
  printf("D: %lf\n", k_d * (error - old_error) / TS);
  printf("PID: %lf\n", -out);
  
  return -out;
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
