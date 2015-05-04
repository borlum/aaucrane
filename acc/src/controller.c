#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

#include <pthread.h>
#include <fcntl.h>
#include <mqueue.h>

#ifdef RTAI
#include <rtai_lxrt.h>
#endif

#ifndef TEST
#include <libcrane.h>
#endif
#include <acc.h>
#include <controller.h>
#include <compensator.h>

#ifdef RTAI
RT_TASK *rt_x_axis_controller;
RT_TASK *rt_y_axis_controller;
#endif

void *task_x_axis_controller(void * argc)
{
  int hit_count = 0;
  int received_new_ref = 0;

  double x_ref = 0;
  double out = 0;

  mqd_t input, output;
  char * input_buffer = (char *)malloc(BUFFER_SIZE);
  input = mq_open(Q_TO_X, O_RDONLY | O_NONBLOCK);
  output = mq_open(Q_FROM_X, O_WRONLY);

#ifdef RTAI
  /*Transform to RTAI task*/
  RTIME period = nano2count(X_SAMPLE_TIME_NS);
  if(!(rt_x_axis_controller = rt_task_init_schmod(nam2num("rt_x_axis_controller"), 1, 0, 0, SCHED_FIFO, 0))){
    printf("Could not start rt_x_axis_controller\n");
    exit(42);
  }
  rt_task_make_periodic(rt_x_axis_controller, rt_get_time() + period, period);
  rt_make_hard_real_time();
  printf("Started rt_x_axis_controller\n");
#endif

  printf("Waiting for ref..\n");
  while(mq_receive(input, input_buffer, BUFFER_SIZE, 0) < 0) {
    printf("Inside while\n");
    if (errno != EAGAIN){
      printf("[X]: error %d, %s\n", errno, strerror(errno));
    }
#ifdef RTAI
    rt_task_wait_period();
#endif
  }

  memcpy(&x_ref, input_buffer, sizeof(double));
  printf("[X]: New x_ref = %.3f\n", x_ref);
  received_new_ref = 1;
  init_ramp(x_ref);

  printf("Before while\n");
  while (1) {
    /*If new reference in queue*/
    if (mq_receive(input, input_buffer, BUFFER_SIZE, 0) > 0) {
      memcpy(&x_ref, input_buffer, sizeof(double));
      printf("[X]: New x_ref = %.3f\n", x_ref);
      received_new_ref = 1;
      init_ramp(x_ref);
    }
    /*EAGAIN = No new data available*/
    else if (errno != EAGAIN){
      printf("[X]: error %d, %s\n", errno, strerror(errno));
    }

    out = pid_get_controller_output(x_ref);
    
    /*Settled?*/
    double err = x_ref - get_xpos();
    /*X inside error band? Angle inside error band? Velocity = 0?*/
    if ( (fabs(err) < X_ERR_BAND) && (get_motorx_velocity() == 0) && (fabs(get_angle()) < ANGLE_ERR_BAND) ) {
      /*Has this happened more than SETTLE_HITS times?*/
      if( ((hit_count++) >= SETTLE_HITS) && received_new_ref ) {
        /*Settled! Allow for new reference and reset hit counter!*/
        received_new_ref = 0;
        hit_count = 0;
        /*Send msg that we have settled!*/
        int msg = 1;
        printf("[X]: DONE @ %lf\n", get_xpos());
        if (mq_send(output, (char *)&msg, sizeof(int), 0) == -1)
          printf("%s\n", strerror(errno));
      }
    } else {
      hit_count = 0;
    }
    
    run_motorx(out);

#ifdef RTAI
    rt_task_wait_period();
#endif
  }
}

void *task_y_axis_controller(void * argc)
{
  int hit_count = 0;
  int received_new_ref = 0;
  
  double y_ref = 0;
  double out = 0;

  mqd_t input, output;
  char* input_buffer = (char*) malloc(BUFFER_SIZE);
  input = mq_open(Q_TO_Y, O_RDONLY | O_NONBLOCK);
  output = mq_open(Q_FROM_Y, O_WRONLY);

#ifdef RTAI
  /*Transform to RTAI task*/
  RTIME period = nano2count(Y_SAMPLE_TIME_NS);
  if(!(rt_y_axis_controller = rt_task_init_schmod(nam2num("rt_y_axis_controller"), 2, 0, 0, SCHED_FIFO, 0))) {
    printf("Could not start rt_y_axis_controller\n");
    exit(42);
  }
  rt_task_make_periodic(rt_y_axis_controller, rt_get_time() + period, period);
  rt_make_hard_real_time();
  printf("Started rt_y_axis_controller\n");
#endif


  while(mq_receive(input, input_buffer, BUFFER_SIZE, 0) < 0) {
    if (errno != EAGAIN){
      printf("[Y]: error %d, %s\n", errno, strerror(errno));
    }
#ifdef RTAI
    rt_task_wait_period();
#endif
  }

  memcpy(&y_ref, input_buffer, sizeof(double));
  printf("[Y]: New y_ref = %.3f\n", y_ref);
  received_new_ref = 1;
  init_ramp(y_ref);

  
  while (1) {
    /*If new reference in queue*/
    if (mq_receive(input, input_buffer, BUFFER_SIZE, 0) > 0) {
      memcpy(&y_ref, input_buffer, sizeof(double));
      printf("[Y]: New y_ref = %.3f\n", y_ref);
      received_new_ref = 1;
    }
    /*EAGAIN = No new data available*/
    else if (errno != EAGAIN) {
      printf("[Y]: error %d, %s\n", errno, strerror(errno));
    }

    /*Settled?*/
    double err = y_ref - get_ypos();
    /*X inside error band? Angle inside error band? Velocity = 0?*/
    if ( (fabs(err) < Y_ERR_BAND) && (get_motory_velocity() == 0) ) {
      printf("HIT\n")
      /*Has this happened more than SETTLE_HITS times?*/
      if( ((hit_count++) >= SETTLE_HITS) && received_new_ref ) {
	printf("Should stop now\n")
        /*Settled! Allow for new reference and reset hit counter!*/
        received_new_ref = 0;
        hit_count = 0;
        /*Send msg that we have settled!*/
        int msg = 1;
        printf("[Y]: DONE @ %lf\n", get_ypos());
        if (mq_send(output, (char *)&msg, sizeof(int), 0) == -1)
          printf("%s\n", strerror(errno));
      }
    } else {
      hit_count = 0;
    }

    out = position_controller_y(err);

    run_motory(out);
#ifdef RTAI
    rt_task_wait_period();
#endif
  }
}
