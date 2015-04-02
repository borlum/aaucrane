#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
// #include <libcrane.h>
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
#ifdef TEST
  int new = 0;
#endif
  mqd_t input;
  mqd_t output;

  input = mq_open(TOX, O_RDONLY | O_NONBLOCK);
  output = mq_open(TOM, O_WRONLY);

  char * input_buffer = (char *)malloc(sizeof(MSG_SIZE));

  double x_ref = 0;
  double x_pos = 0;
  double x_err = 0;
  double out = 0;

  while (1) {
    if (mq_receive(input, input_buffer, MSG_SIZE, 0) > 0) {
      memcpy(&x_ref, input_buffer, sizeof(double));
      printf("[X] New x_ref = %f\n", x_ref);
#ifdef TEST
      new = 1;
#endif
    }
    else if (errno != EAGAIN){ /* Ingen ting i køen */
      printf("[X]: error %d, %s\n", errno, strerror(errno));
    }
#ifndef TEST
    x_pos = get_xpos();
    x_err = x_ref - x_pos;

    if (abs(x_err) < X_ERR_BAND) {
      /*Settled*/
      int msg = 1;
      mq_send(output, (char *)&msg, sizeof(int), 0);
    }

    out = x_err * C2;
    run_motorx(out);
#else
    if(new){
      new = 0;
      int msg = 1;
      usleep(1000 * 1000);
      mq_send(output, (char*) &msg, sizeof(int), 0);
    }
#endif
  }
}

void *ycontroller()
{
#ifdef TEST
  int new = 0;
#endif

  mqd_t input;
  mqd_t output;

  input = mq_open(TOY, O_RDONLY | O_NONBLOCK);
  output = mq_open(TOM, O_WRONLY);

  char* input_buffer = (char*) malloc(sizeof(double));

  double y_ref = 0;
  double y_pos = 0;
  double y_err = 0;
  double out = 0;

  while (1) {
    if (mq_receive(input, input_buffer, MSG_SIZE, 0) > 0) {
      memcpy(&y_ref, input_buffer, sizeof(double));
      printf("[Y] New y_ref = %f\n", y_ref);
#ifdef TEST
      new = 1;
#endif
    }
    else if (errno != EAGAIN){ /* Ingen ting i køen */
      printf("[Y]: error %d, %s\n", errno, strerror(errno));
    }
#ifndef TEST
    y_pos = get_ypos();
    y_err = y_ref - y_pos;

    if (abs(y_err) < Y_ERR_BAND) {
      /*Settled*/
      int msg = 2;
      mq_send(output, (char *)&msg, sizeof(int), 0);
    }

    out = y_err * C3;
    run_motory(out);
#else
    if(new){
      new = 0;
      int msg = 2;
      mq_send(output, (char*) &msg, sizeof(int), 0);
    }
#endif
  }
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

  
  input = mq_open(TOM, O_RDONLY | O_CREAT, 0664, &attr);
  output_x = mq_open(TOX, O_WRONLY | O_CREAT, 0664, &attr);
  output_y = mq_open(TOY, O_WRONLY | O_CREAT, 0664, &attr);

  char * input_buffer = malloc(sizeof(double));

  struct command* commands = args;

  mq_send(output_x, (char*) &(commands->x1), sizeof(double), 0);

  if (mq_receive(input, input_buffer, MSG_SIZE, 0) > 0) {
    tmp = (int) *input_buffer;
    printf("[C] FROM INPUT QUEUE: %d\n", tmp);
  }else{
    printf("[C]: Error %d: %s\n", errno, strerror(errno));
  }

  if (tmp == 1) {
    mq_send(output_y, (char *)&(commands->y1), sizeof(double), 0);
  }


  if (mq_receive(input, input_buffer, MSG_SIZE, 0) > 0) {
    tmp = (int) *input_buffer;
    printf("[C] FROM INPUT QUEUE: %d\n", tmp);
  }

  if (tmp == 2) {
#ifndef TEST
    enable_magnet();
#endif
    mq_send(output_y, (char *)&(commands->yc), sizeof(double), 0);
  }

  if (mq_receive(input, input_buffer, MSG_SIZE, 0) > 0) {
    tmp = (int) *input_buffer;
    printf("[C] FROM INPUT QUEUE: %d\n", tmp);
  }

  if (tmp == 2) {
    mq_send(output_x, (char *)&(commands->x2), sizeof(double), 0);
  }

  if (mq_receive(input, input_buffer, MSG_SIZE, 0) > 0) {
    tmp = (int) *input_buffer;
    printf("[C] FROM INPUT QUEUE: %d\n", tmp);
  }

  if (tmp == 1) {
    mq_send(output_y, (char *)&(commands->y2), sizeof(double), 0);
  }

  if (mq_receive(input, input_buffer, MSG_SIZE, 0) > 0) {
    tmp = (int) *input_buffer;
    printf("[C] FROM INPUT QUEUE: %d\n", tmp);
  }

  if (tmp == 2) {
#ifndef TEST
    disable_magnet();
#endif
  }

  double nul = 0;
  mq_send(output_x, (char*) &nul, sizeof(double), 0);
  mq_receive(input, input_buffer, MSG_SIZE, 0);
  mq_send(output_y, (char*) &nul, sizeof(double), 0);
  mq_receive(input, input_buffer, MSG_SIZE, 0);
  
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
  usleep(1000 * 100); /* Must create queues */
  pthread_create(&thread_xcontroller, NULL, &xcontroller, NULL);
  pthread_create(&thread_ycontroller, NULL, &ycontroller, NULL);

  while(1) {    
    usleep(1000 * 100);
  }  
  
}
