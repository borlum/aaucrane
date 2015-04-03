#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#ifndef TEST
#include <libcrane.h>
#endif

#include <pthread.h>
#include <fcntl.h>
#include <mqueue.h>
#include <string.h>
#include <stdio.h>

#define X_ERR_BAND 0.015
#define Y_ERR_BAND 0.005
#define C2 26.3
#define C3 80

#define TOX "/tox1"
#define TOY "/toy1"
#define TOM "/tom1"

#define MSG_SIZE 8

pthread_t thread_xcontroller;
pthread_t thread_ycontroller;
pthread_t thread_controller;

struct command
{
  double x1;
  double x2;
  double y1;
  double y2;
  double yc;
};

void *xcontroller()
{
  mqd_t input;
  mqd_t output;

  input = mq_open(TOX, O_RDONLY | O_NONBLOCK);
  output = mq_open(TOM, O_WRONLY);

  char * input_buffer = (char *)malloc(sizeof(MSG_SIZE));

  int new_ref = 0;
  double x_ref = 0;
  double x_pos = 0;
  double x_err = 0;
  double out = 0;

  while (1) {
    if (mq_receive(input, input_buffer, MSG_SIZE, 0) > 0) {
      memcpy(&x_ref, input_buffer, sizeof(double));
      printf("[X] New x_ref = %.3f\n", x_ref);
      new_ref = 1;
    }
    else if (errno != EAGAIN){ /* Ingen ting i køen */
      printf("[X]: error %d, %s\n", errno, strerror(errno));
    }
#ifndef TEST
    x_pos = get_xpos();
    x_err = x_ref - x_pos;

    if (abs(x_err) < X_ERR_BAND && new_ref) {
      /*Settled*/
      new_ref = 0;
      int msg = 1;
      mq_send(output, (char *)&msg, sizeof(int), 0);
    }
    out = x_err * C2;
    run_motorx(out);
#else
    if(new_ref){
      new_ref = 0;
      int msg = 1;
      mq_send(output, (char *)&msg, sizeof(int), 0);
    }
#endif
  }
  usleep(1000 * 1000);
}

void *ycontroller()
{
  mqd_t input;
  mqd_t output;

  input = mq_open(TOY, O_RDONLY | O_NONBLOCK);
  output = mq_open(TOM, O_WRONLY);

  char* input_buffer = (char*) malloc(sizeof(double));

  int new_ref = 0;  
  double y_ref = 0;
  double y_pos = 0;
  double y_err = 0;
  double out = 0;

  while (1) {
    if (mq_receive(input, input_buffer, MSG_SIZE, 0) > 0) {
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

    if (abs(y_err) < Y_ERR_BAND && new_ref) {
      /*Settled*/
      new_ref = 0;
      int msg = 2;
      mq_send(output, (char *)&msg, sizeof(int), 0);
    }

    out = y_err * C3;
    run_motory(out);
#else
    if(new_ref){
      new_ref = 0;
      int msg = 2;
      mq_send(output, (char*) &msg, sizeof(int), 0);
    }
#endif
  }
  usleep(1000 * 1000);
}

void *controller(void * args)
{
  printf("Starting controller task\n");
  mqd_t input;
  mqd_t output_x;
  mqd_t output_y;
  int tmp;

  struct mq_attr attr;  
  attr.mq_flags = 0;  
  attr.mq_maxmsg = 10;  
  attr.mq_msgsize = sizeof(double);  
  attr.mq_curmsgs = 0;  

  
  if ( (input = mq_open(TOM, O_RDONLY | O_CREAT, 0664, &attr)) == -1)
    printf("[C] ERROR: %s", strerror(errno));
  if ( (output_x = mq_open(TOX, O_WRONLY | O_CREAT, 0664, &attr)) == -1)
    printf("[C] ERROR: %s", strerror(errno));
  if ( (output_y = mq_open(TOY, O_WRONLY | O_CREAT, 0664, &attr)) == -1)
    printf("[C] ERROR: %s", strerror(errno));

  char * input_buffer = malloc(sizeof(double));

  struct command* commands = args;

  /* Move to x1 */
  mq_send(output_x, (char*) &(commands->x1), sizeof(double), 0);
  mq_receive(input, input_buffer, MSG_SIZE, 0);
  printf("[C] X moved to %.3f\n", commands->x1);

  /* Move to y1 */
  mq_send(output_y, (char *)&(commands->y1), sizeof(double), 0);
  mq_receive(input, input_buffer, MSG_SIZE, 0);
  printf("[C] Y moved to: %.3f\n", commands->y1);

  /* Pick up container */
  printf("[C] Picking up container @ (%.3f, %.3f)\n", commands->x1, commands->y1);
#ifndef TEST
    enable_magnet();
#endif

  /* Move to carry height */
  mq_send(output_y, (char *)&(commands->yc), sizeof(double), 0);
  mq_receive(input, input_buffer, MSG_SIZE, 0);
  printf("[C] In carrying height (%.3fm)\n", commands->yc);
  
  /* Move to x2 */
  mq_send(output_x, (char *)&(commands->x2), sizeof(double), 0);
  mq_receive(input, input_buffer, MSG_SIZE, 0);
  printf("[C] X moved to: %.3f\n", commands->x2);

  /* Move to y2 */
  mq_send(output_y, (char *)&(commands->y2), sizeof(double), 0);
  mq_receive(input, input_buffer, MSG_SIZE, 0);
  printf("[C] Y moved to: %.3f\n", commands->y2);

  /* Dropping container */
  printf("[C] Dropping container\n");
  if (tmp == 2) {
#ifndef TEST
    disable_magnet();
#endif
  }

  /* Move to start (0,0) */
  double nul = 0;
  mq_send(output_x, (char*) &nul, sizeof(double), 0);
  mq_receive(input, input_buffer, MSG_SIZE, 0);
  mq_send(output_y, (char*) &nul, sizeof(double), 0);
  mq_receive(input, input_buffer, MSG_SIZE, 0);


  /* Power down */
#ifndef TEST
  run_motorx(0);
  run_motory(0);
#endif
  exit(0);
}

int main(int argc,char* argv[]){  
  if (argc != 5) {
    printf("usage: %s <x1 y1 x2 y2>", argv[0]);
    exit(1);
  }

  struct command commands = {
    .x1 = atof(argv[1]),
    .y1 = atof(argv[2]),
    .x2 = atof(argv[3]),
    .y2 = atof(argv[4]),
    .yc = 0.5
  };

#ifndef TEST
  initialize_crane();  
  run_motorx(0);
  run_motory(0);
#endif

  pthread_create(&thread_controller, NULL, &controller, &commands);
  pthread_create(&thread_xcontroller, NULL, &xcontroller, NULL);
  pthread_create(&thread_ycontroller, NULL, &ycontroller, NULL);

  while(1) {    
    usleep(1000 * 100);
  }  
  
}
