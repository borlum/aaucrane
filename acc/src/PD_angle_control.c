
#ifndef TEST
#include <libcrane.h>
#endif

#include <filter.h>

#define REF_ARR_SZ 8000

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
