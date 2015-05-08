#include <stdlib.h>
#include <stdio.h>

#include <rtai_lxrt.h>

#include <libcrane.h>
#include <controller.h>

#define SAMPLE_TIME_NS 1E6
#define SAMPLE_TIME_S SAMPLE_TIME_NS * 1E-9

RT_TASK *rt_simple_controller;
pthread_t thread_simple_controller, thread_logger;

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

  double angle_err, pos_err, vel_err;
  double angle_out, pos_out, out;

  double prev_angle_err = 0;
  double velocity_sum   = 0;
  
/*  
  double angle_kp = 2.45;
  double angle_kd = .02;
  double pos_kp   = 30;
  double vel_kp   = 1;
  double vel_ki   = 20;
*/
  double angle_kp = 1;
  double angle_kd = 0;
  double angle_k  = 10;
  double pos_kp   = 2.5;
  double vel_kp   = 3;

  printf("REF: %lf\n", pos_ref);

  while(1){
    angle_err = angle_ref - get_angle();
    pos_err   = pos_ref   - get_xpos();
    
    angle_out = angle_err;
//    angle_out = angle_kp * angle_err + (angle_kd * (angle_err - prev_angle_err) / SAMPLE_TIME_S);

    pos_out   = pos_kp * (pos_err);

//    vel_err = pos_out - get_motorx_velocity();
    vel_err = pos_out - angle_out - get_x_velocity();

    out = vel_kp * vel_err ; //+ vel_ki * velocity_sum * SAMPLE_TIME_S;
    
    printf("===================\n");
    printf("angle_err   : %.3lf\n", angle_err);
    printf("angle_out   : %.3lf\n", angle_out);
    printf("pos_err     : %.3lf\n", pos_err);
    printf("pos_out     : %.3lf\n", pos_out);
    printf("velocity    : %.3lf\n", get_motorx_velocity());
    printf("out         : %.3lf\n", out);
    printf("===================\n");

    run_motorx(out);

    prev_angle_err = angle_err;
    velocity_sum = velocity_sum + vel_err;
    
    rt_task_wait_period();
  }
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
