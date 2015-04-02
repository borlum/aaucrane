#include <stdlib.h>
#include <unistd.h>
#include <libcrane.h>
#include <pthread.h>
#include <fcntl.h>
#include <mqueue.h>
#include <string.h>

#define X_ERR_BAND 0.015
#define Y_ERR_BAND 0.005
#define C2 26.3
#define C3 80

#define TOX "TOX"
#define TOY "TOY"
#define TOM "TOM"


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
  printf("Starting xcontroller task\n");
  mqd_t input;
  mqd_t output;

  input = mq_open(TOX, O_RDONLY|O_NONBLOCK);
  output = mq_open(TOM, O_WRONLY);

  char * input_buffer = (char *)malloc(sizeof(double));

  double x_ref = 0;
  double x_pos = 0;
  double x_err = 0;
  double out = 0;

  while (1) {
    if (mq_receive(input, input_buffer, sizeof(double), 0) > 0) {
      memcpy(&x_ref, input_buffer, sizeof(double));
      printf("New x_ref = %f", x_ref);
    }

    x_pos = get_xpos();
    x_err = x_ref - x_pos;

    if (abs(x_err) < X_ERR_BAND) {
      /*Settled*/
      int msg = 1;
      mq_send(output, (char *)&msg, sizeof(int), 0);
    }

    out = x_err * C2;
    run_motorx(out);
  }
}

void *ycontroller()
{
  printf("Starting ycontroller task\n");
  mqd_t input;
  mqd_t output;

  input = mq_open(TOY, O_RDONLY|O_NONBLOCK);
  output = mq_open(TOM, O_WRONLY);

   char* input_buffer = (char*) malloc(sizeof(double));

  double y_ref = 0;
  double y_pos = 0;
  double y_err = 0;
  double out = 0;

  while (1) {
    if (mq_receive(input, input_buffer, sizeof(double), 0) > 0) {
      memcpy(&y_ref, input_buffer, sizeof(double));
      printf("New y_ref = %f", y_ref);
    }

    y_pos = get_ypos();
    y_err = y_ref - y_pos;

    if (abs(y_err) < Y_ERR_BAND) {
      /*Settled*/
      int msg = 2;
      mq_send(output, (char *)&msg, sizeof(int), 0);
    }

    out = y_err * C3;
    run_motory(out);
  }
}

void *controller(void * args)
{
  printf("Starting controller task\n");
  mqd_t input;
  mqd_t output_x;
  mqd_t output_y;
  int tmp;

  input = mq_open(TOM, O_RDONLY);
  output_x = mq_open(TOX, O_WRONLY);
  output_y = mq_open(TOY, O_WRONLY);

  char * input_buffer = malloc(sizeof(int));

  struct command* commands = args;

  mq_send(output_x, (char *)&(commands->x1), sizeof(double), 0);

  if (mq_receive(input, input_buffer, sizeof(int), 0) > 0) {
    tmp = (int)input_buffer;
    printf("VI FIK: %d\n", tmp);
  }

  if (tmp == 1) {
    mq_send(output_y, (char *)&(commands->y1), sizeof(double), 0);
  }

  if (mq_receive(input, input_buffer, sizeof(int), 0) > 0) {
    tmp = (int)input_buffer;
    printf("VI FIK: %d\n", tmp);
  }

  if (tmp == 2) {
    enable_magnet();
    mq_send(output_y, (char *)&(commands->yc), sizeof(double), 0);
  }

  if (mq_receive(input, input_buffer, sizeof(int), 0) > 0) {
    tmp = (int)input_buffer;
    printf("VI FIK: %d\n", tmp);
  }

  if (tmp == 2) {
    mq_send(output_x, (char *)&(commands->x2), sizeof(double), 0);
  }

  if (mq_receive(input, input_buffer, sizeof(int), 0) > 0) {
    tmp = (int)input_buffer;
    printf("VI FIK: %d\n", tmp);
  }

  if (tmp == 1) {
    mq_send(output_y, (char *)&(commands->y2), sizeof(double), 0);
  }

  if (mq_receive(input, input_buffer, sizeof(int), 0) > 0) {
    tmp = (int)input_buffer;
    printf("VI FIK: %d\n", tmp);
  }

  if (tmp == 2) {
    disable_magnet();
  }

  double nul = 0;
  mq_send(output_x, (char*) &nul, sizeof(double), 0);
  mq_send(output_y, (char*) &nul, sizeof(double), 0);
  usleep(5000);
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
  
  initialize_crane();  
  run_motorx(0);
  run_motory(0);

  pthread_create(&thread_xcontroller, NULL, &xcontroller, NULL);
  pthread_create(&thread_ycontroller, NULL, &ycontroller, NULL);
  pthread_create(&thread_controller, NULL, &controller, &commands);

  while(1) {    
    usleep(1000 * 100);
  }  
  
}
