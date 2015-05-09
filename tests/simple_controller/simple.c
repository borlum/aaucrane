#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <rtai_lxrt.h>

#include <libcrane.h>
#include <controller.h>

#define SAMPLE_TIME_NS 10E6
#define SAMPLE_TIME_S SAMPLE_TIME_NS * 1E-9

RT_TASK *rt_simple_controller;
pthread_t thread_simple_controller, thread_logger;

int current_index = 0;
double ref_arr[10000];

void init_rtai(){
  RTIME period = nano2count(SAMPLE_TIME_NS); 
  if(!(rt_simple_controller = rt_task_init_schmod(nam2num("controller"), 1, 0, 0, SCHED_FIFO, 0))){
    printf("Could not start task\n");
    exit(42);
  }
  rt_task_make_periodic(rt_simple_controller, rt_get_time() + period, period);
  rt_make_hard_real_time();
}

void *simple_controller(void *arg){
  init_rtai();
  double pos_ref = *((double*)arg);
  double angle_ref = 0;  
  double vel;
  
  double angle_out, prev_angle_out;
  double angle_err, prev_angle_err;
  double pos_err;

  
  double pos_out, out;

  /* for hack */
  int sign;
  init_ramp(pos_ref);
  printf("REF: %lf\n", pos_ref);

  /* Morten Tester */
  while(1){
    angle_err = angle_ref - get_angle();
    pos_err   = ref_arr[current_index] - get_xpos();
    vel       = get_x_velocity();

    /* Angle Controller */
    angle_out = 74.91 * angle_err - 70.55 * prev_angle_err + 0.8182 * prev_angle_out;
    angle_out *= -1;
    
    prev_angle_err = angle_err;
    prev_angle_out = angle_out;


    /* Pos Controller */
    pos_out = pos_err * 1;

    if(fabs(pos_err) < 0.2 && fabs(pos_err)> 0.005){
      if(pos_out < 0)
	sign = -1;
      else
	sign = 1;
      pos_out = 0.15 * sign;
    }	
    /* Vel out */
    out = (pos_out + angle_out - vel) * 5;


    if(current_index < (nr_of_ref - 1)) {
      current_index++;
    }

    printf("===================\n");
    printf("pos_err     : %.3lf\n", pos_err);
    printf("pos_out     : %.3lf\n", pos_out);
    printf("angle_err   : %.3lf\n", angle_err);
    printf("angle_out   : %.3lf\n", angle_out);
    printf("out         : %.3lf\n", out);
    printf("===================\n");

    run_motorx(out);
    rt_task_wait_period();

  }
}

void init_ramp(double x_ref){
  double step;
  step = x_ref-get_xpos();
  nr_of_ref = ramp_maker(step);
  current_index = 0;
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


int main(int argc, char *argv[]){
  double ref;

  initialize_crane();
  run_motorx(0);

  printf ("Enter desired position: <x>:\n");
  scanf("%lf", &ref);
  
  init_logger("/var/www/html/data/simple", sizeof("/var/www/html/data/simple"));
  pthread_create(&thread_logger, NULL, task_logger, NULL);  
  enable_logger();
  
  pthread_create(&thread_simple_controller, NULL, simple_controller, &ref);

  while(1){
    usleep(10000);
  }
}
