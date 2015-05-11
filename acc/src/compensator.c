#include <stdio.h>
#include <libcrane.h>
#include "compensator.h"
#include <math.h>

#define RAMP

/*RAMP STUFF*/
#define REF_ARR_SZ 8000
size_t nr_of_ref;
double ref_arr[REF_ARR_SZ];
int current_index = 0;

double angle_controller(double angle_err){
  static double prev_angle_err;
  static double prev_angle_out;

  double angle_out;

  if (libcrane_is_loaded() && fabs(angle_err) < 0.03 ) {
    angle_out = 0;
    return angle_out;
  }

  if (!libcrane_is_loaded() && fabs(angle_err) < 0.04 ) {
    angle_out = 0;
    return angle_out;
  }

  angle_out = 74.91 * angle_err - 70.55 * prev_angle_err + 0.8182 * prev_angle_out;

  angle_out *= -1;

  prev_angle_err = angle_err;
  prev_angle_out = angle_out;
  
  return angle_out;
}

double position_controller_x(double error){
  static double k_p = 1.2;

  int sign;
  if(fabs(error) < 0.15 && fabs(error) > 0.008){
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
    k_p = 25;
  } else if (error < 0) {
    k_p = 150;
  }

  return error * k_p;
}

double get_controller_output(double ref){
  double out, angle_out, pos_out, pos_err;
  
/*STEP or RAMP?*/
#ifdef RAMP
  pos_err = ref_arr[current_index] - get_xpos();
  if(current_index < (nr_of_ref - 1)) {
    current_index++;
  }
#else
  pos_err = ref - get_xpos();
#endif

  
  pos_out   = position_controller_x(pos_err);
  angle_out = angle_controller(-get_angle());

  /*CRAZY HACKZ*/
  if ( fabs(pos_err) < 0.05 ) {
    angle_out = angle_out * 0.5;
  }

  /*MERE GAIN U. CONTAINER*/
  if (!libcrane_is_loaded()) {
    angle_out = angle_out * 2;
  }

  out = velocity_controller_x(angle_out + pos_out - get_x_velocity());
  
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
  } else if(step < 0){
    for(i = 0; i >= step - 0.005; i -= speed){
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