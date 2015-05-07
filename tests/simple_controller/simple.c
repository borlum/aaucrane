#include <stdlib.h>
#include <stdio.h>

#include <rtai_lxrt.h>

#include <libcrane.h>
#include <controller.h>

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

  double angle_err, pos_err;
  double angle_out, out;

  double prev_angle_err = 0;
  
  double angle_kp = 4, angle_kd = 50;
  double pos_kp = 10;

  printf("REF: %lf\n", pos_ref);

  while(1){
    angle_err = angle_ref - get_angle();
    pos_err = pos_ref - get_xpos();
    
    angle_out = angle_kp * angle_err + angle_kd * ((angle_err - prev_angle_err) / SAMPLE_TIME_S);
    out = pos_kp * (pos_err - angle_out);
    
    printf("===================\n");
    printf("angle_err   : %.3lf\n", angle_err);
    printf("pos_err     : %.3lf\n", pos_err);
    printf("angle_out   : %.3lf\n", angle_out);
    printf("out         : %.3lf\n", out);
    printf("===================\n");

    run_motorx(out);

    prev_angle_err = angle_err;
    
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
