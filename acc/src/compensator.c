#include <stdio.h>
#include <libcrane.h>
#include "compensator.h"
#include <math.h>

//#define RAMP

/*RAMP STUFF*/
#define REF_ARR_SZ 8000
size_t nr_of_ref;
double ref_arr[REF_ARR_SZ];
int current_index = 0;

double angle_controller(double error){
  static double prev_err;
  static double prev_out;

  double out;

  printf("ANGLE ERRRRRR: %lf\n", error);
  
  /*#31: CRAZY ANG HACKZ 2*/
  /*Burde være løst i settle condition*/
  if ( fabs(error) < 0.03 ) {
    out = 0;
    return out;
  }

  out = 146 * error - 137.5 * prev_err + 0.7391 * prev_out;

  out *= -1;

  prev_err = error;
  prev_out = out;
  
  return out;
}

double position_controller_x(double error){
  static double k_p = 1.2;
  
  /*#27: CRAZY POS. HACKZ*/
  int sign;
  if(fabs(error) < 0.15 && fabs(error) > 0.01){
    if(error < 0)
      sign = -1;
    else
      sign = 1;
    error = 0.15 * sign;
  } else if (fabs(error) < 0.005) {
    error = 0;
  }

  return error * k_p;
}

double velocity_controller_x(double error){
  static double k_p = 5;
  return error * k_p;
}

double position_controller_y(double error){
  double k_p = 0;

  /*UP = negative error, DOWN = positive error*/
  if (error > 0) {
    if (libcrane_is_loaded()) {
      k_p = 15;
    } else {
      k_p = 1;
    }
  } else if (error < 0) {
    if (libcrane_is_loaded()) {
      k_p = 150;
    } else {
      k_p = 1;
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
  
  pos_out = position_controller_x(pos_err);
  ang_out = angle_controller(ang_err);
  out = velocity_controller_x(ang_out + pos_out - get_x_velocity());
  
  return out;
}

int ramp_maker(double step){
  double i,  speed = .008, off_set = get_xpos(); //speed is in m/ms
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

  return j;
}

void init_ramp(double x_ref){
  double step;
  step = x_ref - get_xpos();
  nr_of_ref = ramp_maker(step);
  current_index = 0;
}
