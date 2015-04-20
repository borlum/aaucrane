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
#include "acc.h"
#include "controller.h"
#include "filter.h"

#ifdef RTAI
RT_TASK *rt_x_axies_controller;
RT_TASK *rt_y_axies_controller;
#endif

void *task_x_axies_controller(void * argc)
{
  int hit_count = 0;
  int new_ref = 0;

  double x_ref = 0, x_pos = 0, x_err = 0, x_err_int = 0;
  double angle_ref = 0, angle_pos = 0, angle_err = 0;
  double velocity_err = 0, velocity = 0;
  double x_velocity = 0;
  double pI = .1;
  double out = 0;


  mqd_t input, output;
  char * input_buffer = (char *)malloc(BUFFER_SIZE);

  input = mq_open(Q_TO_X, O_RDONLY | O_NONBLOCK);
  output = mq_open(Q_FROM_X, O_WRONLY);

#ifdef RTAI
  RTIME period = nano2count(1000 * 1000 * 1); /* really fast! */
  if(!(rt_x_axies_controller = rt_task_init_schmod(nam2num("rt_x_axies_controller"), 1, 0, 0, SCHED_FIFO, 0))){
    printf("Could not start rt_task\n");
    exit(42);
  }
  rt_task_make_periodic(rt_x_axies_controller, rt_get_time() + period, period);
  rt_make_hard_real_time();
  printf("Started rt_x_axies_controller\n");
#endif /* RTIA */

  while (1) {
    if (mq_receive(input, input_buffer, BUFFER_SIZE, 0) > 0) {
      memcpy(&x_ref, input_buffer, sizeof(double));
      printf("[X] New x_ref = %.3f\n", x_ref);
      new_ref = 1;
    }
    else if (errno != EAGAIN){ /* Ingen ting i køen */
      printf("[X]: error %d, %s\n", errno, strerror(errno));
    }
#ifndef TEST
    x_pos = get_xpos();
    x_velocity = get_x_velocity();
    angle_pos = get_angle();
    velocity = get_motorx_velocity();

    //velocity = (x_pos - old_x_pos) * 1/0.0039 * 1/1000;

    angle_err = angle_ref - angle_pos;
    printf("[angle] out: %.2lf\n", angle_err);
    x_err = x_ref - x_pos - angle_controller(angle_err) ;
    printf("[X_pos] out: %.2lf\n", x_pos);
    printf("[X_err] out: %lf\n", x_err);
    out = position_controller_x(x_err) + pI * x_err_int;
    velocity_err = out - velocity;
    //printf("[velocity] out: %lf\n", velocity);
    out = velocity_controller_x(velocity_err);
    printf("[output] : %.3lf\n", out);
    printf("[Integrator value] : %.2lf\n\n", x_err_int);

    x_err_int += x_err;

        if ( (fabs(x_err) < X_ERR_BAND)) {
      /*Settled*/
      hit_count++;
      if(hit_count >= 10000 && new_ref){
        run_motorx(0);
        break;
        new_ref = 0;
        hit_count = 0;
        int msg = 1;
        mq_send(output, (char *)&msg, sizeof(int), 0);
      }
    } else {
      run_motorx(out);
    }
#ifdef RTAI
    rt_task_wait_period();
#endif /* RTAI */
#else /* TEST */
    if(new_ref){
      new_ref = 0;
      int msg = 1;
      mq_send(output, (char *)&msg, sizeof(int), 0);
    }
#endif /* TEST */
  }
}

void *task_y_axies_controller(void * argc)
{
  int hit_count = 0;
  int new_ref = 0;
  double y_ref = 0, y_pos = 0, y_err = 0;

  double out = 0;

  mqd_t input;
  mqd_t output;
  char* input_buffer = (char*) malloc(BUFFER_SIZE);

  input = mq_open(Q_TO_Y, O_RDONLY | O_NONBLOCK);
  output = mq_open(Q_FROM_Y, O_WRONLY);

#ifdef RTAI
  RTIME period = nano2count(1000 * 1000 * 10); /* Not as fast as X */
  if(!(rt_y_axies_controller = rt_task_init_schmod(nam2num("rt_y_axies_controller"), 2, 0, 0, SCHED_FIFO, 0))){
    printf("Could not start rt_task\n");
    exit(42);
  }
  rt_task_make_periodic(rt_y_axies_controller, rt_get_time() + period, period);
  rt_make_hard_real_time();
  printf("Started rt_y_axies_controller\n");
#endif /* RTAI */

  while (1) {
    if (mq_receive(input, input_buffer, BUFFER_SIZE, 0) > 0) {
      memcpy(&y_ref, input_buffer, sizeof(double));
      printf("[Y] New y_ref = %.3f\n", y_ref);
      new_ref = 1;
    }
    else if (errno != EAGAIN){ /* Ingen ting i køen */
      printf("[Y]: error %d, %s\n", errno, strerror(errno));
    }
#ifndef TEST
    y_pos = get_ypos();
    y_err = y_ref - y_pos;

    if (fabs(y_err) < Y_ERR_BAND) {
      /*Settled*/
      hit_count++;
      if(hit_count >= 10 && new_ref){
         new_ref = 0;
         hit_count = 0;
         int msg = 2;
         mq_send(output, (char *)&msg, sizeof(int), 0);
      }
    }

    out = position_controller_y(y_err);
    run_motory(out);
#ifdef RTAI
    rt_task_wait_period();
#endif /* RTAI */
#else /* TEST */
    if(new_ref){
      new_ref = 0;
      int msg = 2;
      mq_send(output, (char*) &msg, sizeof(int), 0);
    }
#endif /* TEST */
  }
}
