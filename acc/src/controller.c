#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>

#include <pthread.h>
#include <fcntl.h>
#include <mqueue.h>
#include <string.h>
#include <stdio.h>

#ifndef TEST
#include <libcrane.h>
#endif
#include "acc.h"
#include "controller.h"
#include "filter.h"


void *rt_x_axies_controller(void * argc)
{
  mqd_t input;
  mqd_t output;

  input = mq_open(Q_TO_X, O_RDONLY | O_NONBLOCK);
  output = mq_open(Q_FROM_X, O_WRONLY);

  char * input_buffer = (char *)malloc(BUFFER_SIZE);

  int hit_count = 0;
  int new_ref = 0;
  double x_ref = 0;
  double x_pos = 0;
  double x_err = 0;

  double angle_ref = 0;
  double angle_pos = 0;
  double angle_err = 0;
  
  double out = 0;

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
    angle_pos = get_angle();
    angle_err = angle_ref - angle_pos;
    x_err = x_ref - x_pos - angle_controller(angle_err);
    
    if ( (fabs(x_err) < X_ERR_BAND)) {
      /*Settled*/
      hit_count++;
      if(hit_count >= 100 && new_ref){
        new_ref = 0;
        hit_count = 0;
        int msg = 1;
        mq_send(output, (char *)&msg, sizeof(int), 0);
      }
    }

    out = position_controller_x(x_err);
    run_motorx(out);
#else
    if(new_ref){
      new_ref = 0;
      int msg = 1;
      mq_send(output, (char *)&msg, sizeof(int), 0);
    }
#endif
  }
}

void *rt_y_axies_controller(void * argc)
{
  mqd_t input;
  mqd_t output;

  input = mq_open(Q_TO_Y, O_RDONLY | O_NONBLOCK);
  output = mq_open(Q_FROM_Y, O_WRONLY);

  char* input_buffer = (char*) malloc(BUFFER_SIZE);

  int hit_count = 0;
  int new_ref = 0;  
  double y_ref = 0;
  double y_pos = 0;
  double y_err = 0;
  double out = 0;

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
#else
    if(new_ref){
      new_ref = 0;
      int msg = 2;
      mq_send(output, (char*) &msg, sizeof(int), 0);
    }
#endif
  }
}
