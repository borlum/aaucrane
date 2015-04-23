
#ifndef TEST
#include <libcrane.h>
#endif

#include <filter.h>

#define REF_ARR_SZ 8000

double angle_integrate = 0, angle_prev = 0;


size_t nr_of_ref;
double ref_arr[REF_ARR_SZ];
int current_index = 0;

void pd_init_controller(double x_ref){
  double step;
  step = x_ref-get_xpos();
  nr_of_ref = ramp_maker(step, ref_arr);
  current_index = 0;
}

double pd_get_controller_output(){
  double angle_pos = 0, out;
  angle_pos = get_angle();
  out = angle_controller(angle_pos);
  out += position_controller_x(ref_arr[current_index] - get_xpos());
  if(current_index < (nr_of_ref - 1)) {
    current_index++;
  }

  return out;
}


double pid_get_controller_output(){
  double angle_pos = 0, out;
  double C2 = 11.6, k = 38, tp = 1, td = .1, ti = 4;
  angle_pos = get_angle();

  out = angle_pos*(k*tp + k * ti * (angle_integrate) * TS + k) + k * td * (angle_prev-angle_pos);

  angle_integrate += angle;
  angle_prev = angle;

  out += C2 * (ref_arr[current_index] - get_xpos());

  if(current_index < (nr_of_ref - 1)) {
    current_index++;
  }

  return out;
}
