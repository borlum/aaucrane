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
/* Controllers */
static const double X_ERR_BAND = 0.015;
static const double Y_ERR_BAND = 0.015;
static const double C1 = 10;
static const double C2 = 26.3;
static const double C3 = 80;

/* Queue related stuff */
static const int buffer_size = 2 * sizeof(crane_cmd_t);
static const char* q_to_x = "/ACC_to_x";
static const char* q_from_x = "/ACC_from_x";

static const char* q_to_y = "/ACC_to_y";
static const char* q_from_y = "/ACC_from_y";

static const char* q_to_c = "/ACC_to_c";
static const char* q_from_c = "/ACC_from_c";

/* Threads */
pthread_t thread_xcontroller;
pthread_t thread_ycontroller;
pthread_t thread_controller;

void *xcontroller()
{
  mqd_t input;
  mqd_t output;

  input = mq_open(q_to_x, O_RDONLY | O_NONBLOCK);
  output = mq_open(q_from_x, O_WRONLY);

  char * input_buffer = (char *)malloc(buffer_size);

  int new_ref = 0;
  double x_ref = 0;
  double x_pos = 0;
  double x_err = 0;

  double angle_ref = 0;
  double angle_pos = 0;
  double angle_err = 0;

  
  double out = 0;

  while (1) {
    if (mq_receive(input, input_buffer, buffer_size, 0) > 0) {
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
    x_err = x_ref - (angle_err * 10) - x_pos;
    
    printf("X_ref: %.3f | X_pos: %.3f | Angle_pos: %.3f\n", x_ref, x_pos, angle_pos);
    
    if ( (fabs(x_err) < X_ERR_BAND) && new_ref) {
      /*Settled*/
      printf("[X] Settled: pos_sensor = %.3f | x_ref = %.3f\n", fabs(x_err), x_ref);
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
    usleep(1000 * 10);
  }
}

void *ycontroller()
{
  mqd_t input;
  mqd_t output;

  input = mq_open(q_to_y, O_RDONLY | O_NONBLOCK);
  output = mq_open(q_from_y, O_WRONLY);

  char* input_buffer = (char*) malloc(buffer_size);

  int new_ref = 0;  
  double y_ref = 0;
  double y_pos = 0;
  double y_err = 0;
  double out = 0;

  while (1) {
    if (mq_receive(input, input_buffer, buffer_size, 0) > 0) {
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

    if (fabs(y_err) < Y_ERR_BAND && new_ref) {
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
}

void *controller(void * args)
{
  printf("Starting controller task\n");
  mqd_t to_x, from_x;
  mqd_t to_y, from_y;
  mqd_t to_c, from_c;

  double reset_pos_flag = 0;
  
  crane_cmd_t* cmd;
  char * buffer = malloc(buffer_size);

  to_x = mq_open(q_to_x, O_WRONLY);
  from_x = mq_open(q_from_x, O_RDONLY);

  to_y = mq_open(q_to_y, O_WRONLY);
  from_y = mq_open(q_from_y, O_RDONLY);
  
  to_c = mq_open(q_to_c, O_RDONLY);
  from_c = mq_open(q_from_c, O_WRONLY);

  
  while(1){
    printf("Crane reday for next command... \n");
    mq_receive(to_c, buffer, buffer_size, 0);
    cmd = (crane_cmd_t*) buffer;
    
    printf("#### NEW COMMAND ####\n");
    printf("GOTO (%.3f, %.3f)\n", cmd->x_ref[0], cmd->y_ref[0]);
    printf("%s magnet\n", cmd->magnet == ENABLE ? "Enable" : "Disable");
    printf("Carry @ %.3f\n", cmd->carry_height);
    printf("GOTO (%.3f, %.3f)\n", cmd->x_ref[1], cmd->y_ref[1]);
    printf("Disable magnet\n");
    printf("#### END COMMAND ####\n");
    
    
    mq_send(to_x, (char *) &(cmd->x_ref[0]), sizeof(cmd->x_ref[0]), 0);
    mq_receive(from_x, NULL, buffer_size, 0);
    printf("[C] Moved to x: %.3f\n", cmd->x_ref[0]);
    
    mq_send(to_y, (char *) &(cmd->y_ref[0]), sizeof(cmd->y_ref[0]), 0);
    mq_receive(from_y, NULL, buffer_size, 0);
    printf("[C] Moved to y: %.3f\n", cmd->y_ref[0]);
    
    if(cmd->magnet == ENABLE){
#ifndef TEST
      enable_magnet();
#else
      printf("Magnet enabled\n");
#endif
    }
    mq_send(to_y, (char *) &(cmd->carry_height), sizeof(cmd->carry_height), 0);
    mq_receive(from_y, NULL, buffer_size, 0);
    printf("[C] Moved to y: %.3f\n", cmd->carry_height);

    mq_send(to_x, (char *) &(cmd->x_ref[1]), sizeof(cmd->x_ref[1]), 0);
    mq_receive(from_x, NULL, buffer_size, 0);
    printf("[C] Moved to x: %.3f\n", cmd->x_ref[1]);

    mq_send(to_x, (char *) &(cmd->y_ref[1]), sizeof(cmd->y_ref[1]), 0);
    mq_receive(from_x, NULL, buffer_size, 0);
    printf("[C] Moved to y: %.3f\n", cmd->y_ref[1]);

    if(cmd->magnet == ENABLE){
#ifndef TEST
      disable_magnet();
#else
      printf("Magnet disabled\n");
#endif
    }
    mq_send(to_y, (char *) &reset_pos_flag, sizeof(reset_pos_flag), 0);
    mq_receive(from_y, NULL, buffer_size, 0);

    mq_send(to_x, (char *) &reset_pos_flag, sizeof(reset_pos_flag), 0);
    mq_receive(from_x, NULL, buffer_size, 0);
    mq_send(from_c, (char *) &cmd, sizeof(cmd), 0);
  }
}

int init(){
#ifndef TEST
  initialize_crane();  
  run_motorx(0);
  run_motory(0);
#endif
  struct mq_attr attr;  
  attr.mq_flags = 0;  
  attr.mq_maxmsg = 10;  
  attr.mq_msgsize = buffer_size;  
  attr.mq_curmsgs = 0;  

  if ( mq_open(q_to_x, O_RDONLY | O_CREAT, 0664, &attr) == -1 ||
       mq_open(q_to_y, O_WRONLY | O_CREAT, 0664, &attr) == -1 ||
       mq_open(q_to_c, O_WRONLY | O_CREAT, 0664, &attr) == -1 ||
       mq_open(q_from_x, O_RDONLY | O_CREAT, 0664, &attr) == -1 ||
       mq_open(q_from_y, O_WRONLY | O_CREAT, 0664, &attr) == -1 ||
       mq_open(q_from_c, O_WRONLY | O_CREAT, 0664, &attr) == -1
     ){
      printf("ERROR: %s\n", strerror(errno));
      return -1;
  }
  
  pthread_create(&thread_xcontroller, NULL, xcontroller, NULL);
  pthread_create(&thread_ycontroller, NULL, ycontroller, NULL);
  pthread_create(&thread_controller, NULL,controller, NULL);
  return 0;
}

int main(int argc,char* argv[]){  
  if( init() == -1)
    exit(-1);
  
  mqd_t to_c, from_c;
  to_c = mq_open(q_to_c, O_WRONLY);
  from_c = mq_open(q_from_c, O_RDONLY);

  double tmp;
  float a;
  crane_cmd_t cmd;
    
  while(1) {
    printf ("Enter a crane command <x1,y1 x2,y2 {1|0}>:\n");
    scanf("%lf,%lf %lf,%lf %d",
	  &(cmd.x_ref[0]), &(cmd.y_ref[0]), &(cmd.x_ref[1]), &(cmd.y_ref[1]), &(cmd.magnet) );

    mq_send(to_c, (char *) &cmd, sizeof(cmd), 0);
    mq_receive(from_c, NULL, buffer_size, 0);
  }  
  
}
