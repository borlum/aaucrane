#include<stdio.h>
#include<libcrane.h>

#include "../include/filter.h"
const static double TS = 0.001;

double angle_integrate = 0, angle_prev = 0;

/*Angle controller 3*/
double pre_error = 0;
double pre_out   = 0;

double angle_controller(double angle_pos){
  double out, TS = .001, angle_windup_val = 1;
  double k, tp, td, ti;

  /* Diffrent parameters depending of container */
  if(get_ctrlpad_magnet_switch()){
    k = 10;
    tp = 0.5;
    td = 0.2;
    ti = 4.1;
  }
  else{
    k = 10;
    tp = 0.5;
    td = 0.2;
    ti = 4.15;
  }
  
  out = angle_pos*(k*tp + k * ti * (angle_integrate) * TS + k) + k * td * (angle_prev-angle_pos);
  angle_integrate += angle_pos;

/*  if(angle_integrate < -angle_windup_val) angle_integrate = -angle_windup_val;
  else if(angle_integrate > angle_windup_val) angle_integrate = angle_windup_val;
*/
  angle_prev = angle_pos;

  return out;
}

double angle_controller2(double angle_pos){
  double out, k_p = 4, td = .06, TS = .001;

  out = k_p * (1 + td * (angle_prev - angle_pos));

  angle_prev = angle_pos;

  return out;
}

double angle_controller3(double error){
  double out = 80 * error - 80 * pre_error + 0.99 * pre_out;
  pre_out = out;
  pre_error = error;
  return out;
}

double position_controller_x(double error){
  double k_p;

  if(get_ctrlpad_magnet_switch)
    k_p = 15;
  else
    k_p = 15;

  return error * k_p;
}

double position_controller_x2(double error){
  double k_p = 25;
  return error * k_p;
}

double position_controller_x3(double error){
  double k_p = 11.6;
  return error * k_p;
}

double position_controller_y(double error){
  double k_p = 49.3, comp;

  if(error > 0) comp = 0;
  else if(error < 0) comp = 0;

  return error * (k_p + comp);
}

double velocity_controller_x(double error){
  double k_p = 50;
  return k_p * error;
}

int ramp_maker(double step, double *ramp_arr){
  double i,  speed = .0005, off_set = get_xpos(); //speed is in m/ms
  int j = 0;

  if(step>0){
    for(i = 0; i<=step; i += speed){
      ramp_arr[j] = i + off_set;
      j++;
    }
  } else if(step < 0){
    for(i = 0; i>=step; i -= speed){
      ramp_arr[j] = i + off_set;
      j++;
    }
  }

  return j;

}
