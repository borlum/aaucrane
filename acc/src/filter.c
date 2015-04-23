#include<stdio.h>
#include<libcrane.h>

#include "../include/filter.h"
const static double TS = 0.001;

double error_sum = 0;
double old_error = 0;

double angle_controller(double error){
  const double k_p = 50;
  const double k_i = 0;
  const double k_d = .2;


  double p = 0, i = 0, d = 0, out = 0;

  p = k_p * error;
  i = 0;
  d = (k_d * (error - old_error)) / TS;


  out = p + d;
  old_error = error;

  return out;
}

double position_controller_x(double error){
  double k_p = 5;
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

int ramp_maker(double step, double *ramp_arr){
  double i,  speed = .0005, off_set = get_xpos();
  int j = 0;

  if(step>0){
    for(i = 0; i<step; i += speed){
      ramp_arr[j] = i + off_set;
      j++;
    }
  } else if(step < 0){
    for(i = 0; i>step; i -= speed){
      printf("J = %d\n", j);
      ramp_arr[j] = i + off_set;
      j++;
    }
  }

  printf("J: %d\n", j);
  return j;

}

/*int ref_controller(double error, double* output, size_t out_len){
  double velocity = 0.5;
  double i = 0;
  double current_pos = get_xpos();
  int j = 0;

  if (error > 0){
    while(i < error){
      output[j] = current_pos + i;
      i += velocity * TS;
      if(++j > out_len)
        return -1;
    }
  } else if (error < 0){
    while(i > error){
      output[j] = current_pos + i;
      i -= velocity * TS;
      if(++j > out_len)
        return -1;
    }
  }
  return j;
}*/
