
#ifndef TEST
#include <libcrane.h>
#endif

#include <filter.h>

#define REF_ARR_SZ 8000


double pos_integrate = 0;

size_t nr_of_ref;
double ref_arr[REF_ARR_SZ];
int current_index = 0;

double x_goal;

void pd_init_controller(double x_ref){
  double step;
  x_goal = x_ref;
  step = x_ref-get_xpos();
  nr_of_ref = ramp_maker(step, ref_arr);
  current_index = 0;
}

double pd_get_controller_output(){
  double angle_pos = 0, out;
  angle_pos = get_angle();
  out = 0; //angle_controller(angle_pos);
  out += position_controller_x(ref_arr[current_index] - get_xpos());
  if(current_index < (nr_of_ref - 1)) {
    current_index++;
  }

  return out;
}


double pid_get_controller_output(){
  double out;
  static int aw  =0;
  double angle = get_angle();

  out = angle_controller(angle, aw);

//  printf("Angle Out: %lf\n", out);

//  printf("Angle: %lf\n", angle);

    out += position_controller_x(x_goal-get_xpos());
//    out += position_controller_x(ref_arr[current_index] - get_xpos());
//  printf("Position Out: %lf\n", out);
  

  if(current_index < (nr_of_ref - 1)) {
    current_index++;
  }

if(out >= 12.5 && out <= -12.5) aw = 1;
  else aw = 0;

  return out;
}


double ld_get_controller_output(double x_ref){
  double out;
  static int aw = 0;
  out = angle_controller3(get_angle(), aw);
  out += position_controller_x3(ref_arr[current_index] - get_xpos());
  
  if(current_index < (nr_of_ref - 1)) {
    current_index++;
  }

if(out >= 12.5 && out <= -12.5) aw = 1;
  else aw = 0;

  return out;
}
