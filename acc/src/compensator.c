#include <stdio.h>
#include <libcrane.h>
#include "compensator.h"
#include <math.h>

//#define RAMP 
#define HACKZ
//#define CASCADE

/*RAMP STUFF*/
#define REF_ARR_SZ 8000
size_t nr_of_ref;
double ref_arr[REF_ARR_SZ];
int current_index = 0;

double angle_controller(double error){
  static double prev_err;
  static double prev_out;

  double out;

#ifdef HACKZ
  if (fabs(error) < 0.03) {
    error = 0;
  }
#endif

#ifdef CASCADE
  /*In theory: 1428, 1372, 1*/
  out = 1428 * error - 1372 * prev_err - 1 * prev_out;
#else
  out = 146 * error - 137.5 * prev_err - 0.7391 * prev_out;
#endif

  prev_err = error;
  prev_out = out;

  out *= -1; /* Virkelig lækkert at vi både vender fortegnet her og nede ved ang_err :) */
  
  return out;
}

double position_controller_x(double error){
#ifdef CASCADE
  double k_p = 0.8;
#else
  static double k_p = 3; /*3.75 in haxx, 1.15 in theory*/
#endif

  return error * k_p;
}

double velocity_controller_x(double error){
#ifdef CASCADE
  static double k_p = 10; /*10 in theory*/
#else
  static double k_p = 6; /*5 in theory*/
#endif

#ifdef HACKZ
  if ( fabs(error) < 0.02 ) {
    return 0;
  }
#endif

  return error * k_p;
}

double position_controller_y(double error){
  double k_p = 0;

  /*UP = negative error, DOWN = positive error*/
  if (error > 0) {
    if (libcrane_is_loaded()) {
      k_p = 15 ;// 26.9; //15
    } else {
      k_p = 25 ;// 42; //25
    }
  } else if (error < 0) {
    if (libcrane_is_loaded()) {
      k_p = 25 ;// 64; //25
    } else {
      k_p = 20 ;// 42; //20
    }
  }

  return error * k_p;
}

double get_controller_output(double ref){
  double out, ang_out, ang_err, pos_out, pos_err;
  
/*STEP or RAMP?*/
#ifdef RAMP
  pos_err = ref_arr[current_index] - get_xpos();
  if(current_index < (nr_of_ref - 1)) {
    current_index++;
  }
#else
  pos_err = ref - get_xpos();
#endif

  ang_err =  -get_angle();
  
#ifdef CASCADE
  ang_out = angle_controller(ang_err);
  pos_out = position_controller_x(ang_out + pos_err);
  out = velocity_controller_x(pos_out - get_x_velocity());
#else
  pos_out = position_controller_x(pos_err);
  ang_out = angle_controller(ang_err);
  out = velocity_controller_x(ang_out + pos_out - get_x_velocity());
#endif

  printf("POS OUT = %+lf \n", pos_out);
  printf("ANG ERR = %+lf \n", ang_err);
  printf("ANG OUT = %+lf \n", ang_out);
  printf("VEL ERR = %+lf \n", ang_out + pos_out - get_x_velocity());
  printf("VEL OUT = %+lf \n", out);
  
  return out;
}

int ramp_maker(double step){
  double i,  speed = .0035, off_set = get_xpos(); //speed is in m/ms
  int j = 0;

  if (step > 0) {
    for(i = 0; i <= step + 0.005; i += speed){
      ref_arr[j] = i + off_set;
      j++;
    }
  } else if (step < 0) {
    for(i = 0; i >= step - 0.005; i -= speed){
      ref_arr[j] = i + off_set;
      j++;
    }
  }
  ref_arr[j++] = step+off_set;
  return j;
}

void init_ramp(double x_ref){
  double step;
  step = x_ref - get_xpos();
  nr_of_ref = ramp_maker(step);
  current_index = 0;
}
