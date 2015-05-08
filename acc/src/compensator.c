#include <stdio.h>
#include <libcrane.h>
#include "compensator.h"
#include <math.h>

#define NEW
#define RAMP 0

/*RAMP STUFF*/
#define REF_ARR_SZ 8000
size_t nr_of_ref;
double ref_arr[REF_ARR_SZ];
int current_index = 0;

double angle_controller(double error, double x_error){
  static double pre_error = 0;
  static double pre_out = 0;
  static double ang_lim = 10;
  int sign;
  double out, k, td, tp;

#ifdef NEW
  /* Alternate Design */  
  k = 5; 
  
  tp = 4;
  
  td = 1;
 
  out = k * (error * tp + ((error-pre_error) * td) / (SAMPLE_TIME_S) );

  if (out < 0) sign = -1;
  else if (out >= 0) sign = 1;

  pre_error = error;
  pre_out = out;

  out = (out * (x_error * 3)); 
  
  if(fabs(out) > ang_lim){
    out = ang_lim * sign; 
  }

#else
  /* After Kirsten */  
  k  =  20;
  tp =  1;
  td =  0.00;

  out = (k*tp * error) + (k*td * (error - pre_error) / SAMPLE_TIME_S);
  printf("=====================\n");
  printf("[C2] pre_error = %lf \n", pre_error);
  printf("[C2] error     = %lf \n", error);
  printf("[C2] P         = %lf \n", k*tp * error);
  printf("[C2] D         = %lf \n", k*td * (error - pre_error) / SAMPLE_TIME_S);
  printf("[C2] OUT       = %lf \n", out);
  printf("=====================\n");
#endif


  return out;
}

double position_controller_x(double error){
  /*W. container*/
  double k_p = 4.75;
#ifdef NEW
  k_p = 5;
  printf("Pos out: %lf\n", error*k_p);
#endif
  /* printf("=====================\n"); */
  /* printf("[C1] error     = %lf \n", error); */
  /* printf("[C1] OUT     = %lf \n", k_p*error); */
  /* printf("=====================\n"); */

  return error * k_p;
}

double position_controller_y(double error){
  double k_p = 49.3;
  return error * k_p;
}

double get_controller_output(double ref){
  double out, angle_out, x_pos_out;
  
  angle_out = angle_controller(get_angle(), ref-get_xpos());
  
  printf("Angle_out: %lf\n", angle_out);

  /*STEP or RAMP?*/
  if (RAMP == 0) {
#ifdef NEW
    x_pos_out = position_controller_x(ref - get_xpos());
    out = angle_out + x_pos_out;
#else
    out = position_controller_x( angle_out + (ref_arr[current_index] - get_xpos()) );	
#endif
  } else {
    out = position_controller_x( angle_out + (ref_arr[current_index] - get_xpos()) );
  }

 // printf("Final Out: %lf\n", out);

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
