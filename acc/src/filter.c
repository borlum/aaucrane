#include<stdio.h>

#include "../include/filter.h"
const static double TS = 1/1000;

static double error_sum = 0;
static double old_error = 0; 

double angle_controller(double error){
  const static double k_p = 45.8;
  const static double k_i = 1;
  const static double k_d = 10;

  
  double out;

  out = k_p * error + k_i * error_sum + k_d * (error - old_error);

  error_sum += error;
  old_error = error;

  printf("#### PID ####\n");
  printf("Error %lf\n", error);
  printf("Error sum %lf\n", error_sum);
  printf("P: %lf\n", k_p * error);
  printf("I: %lf\n", k_p * error_sum);
  printf("D: %lf\n", k_p * k_d * (error - old_error) * TS);
  printf("PID: %lf\n", k_p * out);
  
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
