#include <stdio.h>
#include <libcrane.h>
#include "compensator.h"

#define RAMP 0

/*RAMP STUFF*/
#define REF_ARR_SZ 8000
size_t nr_of_ref;
double ref_arr[REF_ARR_SZ];
int current_index = 0;

double angle_controller(double error){
  static double pre_error = 0;
  double out, k, td, tp;

  /* After Kirsten */
  k  =  10.0;
  tp =  4.0;
  td =  1.0;

  //out =  k*td * ( ( (1/td) * (error - pre_error) / SAMPLE_TIME_S ) + 1);
  
  /*printf("[C2] pre_error = %lf \n", pre_error);
  printf("[C2] error = %lf \n", error);
  printf("[C2] P     = %lf \n", k*td*error);
  printf("[C2] D     = %lf \n", k * (error - pre_error));*/

  out = k*tp * error + k*td * (error - pre_error);

  /*Just P*/
  out = 10 * error;

  printf("[C2] OUT   = %lf \n", out);

  /*Skip, and run proportional instead!*/
  //out = error * 10;

  pre_error = error;

  return out;
}

double position_controller_x(double error){
  /*W. container*/
  double k_p = 3.75;

  /*printf("[C1] error = %lf \n", error);*/
  printf("[C1] OUT     = %lf \n", k_p*error);

  return error * k_p;
}

double position_controller_y(double error){
  double k_p = 49.3;
  return error * k_p;
}

double get_controller_output(double ref){
  double out, angle;
  
  angle = get_angle();
  out   = angle_controller(angle);

  /*STEP or RAMP?*/
  if (RAMP == 0) {
    out = position_controller_x( out + ref - get_xpos() );
  } else {
    out = position_controller_x( out + ref_arr[current_index] - get_xpos() );
  }

  /*Update RAMP*/
  if(current_index < (nr_of_ref - 1)) {
    current_index++;
  }

  return out;
}

int ramp_maker(double step){
  double i,  speed = .0005, off_set = get_xpos(); //speed is in m/ms
  int j = 0;

  if(step>0){
    for(i = 0; i<=step; i += speed){
      ref_arr[j] = i + off_set;
      j++;
    }
  } else if(step < 0){
    for(i = 0; i>=step; i -= speed){
      ref_arr[j] = i + off_set;
      j++;
    }
  }

  return j;
}

void init_ramp(double x_ref){
  double step;
  step = x_ref-get_xpos();
  nr_of_ref = ramp_maker(step);
  current_index = 0;
}
