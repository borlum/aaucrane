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
#include "include/controller.h"

/* Threads */
pthread_t thread_xcontroller;
pthread_t thread_ycontroller;
pthread_t thread_controller;

void *controller(void * args)
{
  printf("Starting controller task\n");
  mqd_t to_x, from_x;
  mqd_t to_y, from_y;
  mqd_t to_c, from_c;

  double reset_pos_flag = 0;
  
  crane_cmd_t* cmd;
  char * buffer = malloc(BUFFER_SIZE);

  to_x = mq_open(Q_TO_X, O_WRONLY);
  from_x = mq_open(Q_FROM_X, O_RDONLY);

  to_y = mq_open(Q_TO_Y, O_WRONLY);
  from_y = mq_open(Q_FROM_Y, O_RDONLY);
  
  to_c = mq_open(Q_TO_C, O_RDONLY);
  from_c = mq_open(Q_FROM_C, O_WRONLY);

  
  while(1){
    printf("Crane reday for next command... \n");
    mq_receive(to_c, buffer, BUFFER_SIZE, 0);
    cmd = (crane_cmd_t*) buffer;
    
    printf("#### NEW COMMAND ####\n");
    printf("GOTO (%.3f, %.3f)\n", cmd->x_ref[0], cmd->y_ref[0]);
    printf("%s magnet\n", cmd->magnet == ENABLE ? "Enable" : "Disable");
    printf("Carry @ %.3f\n", cmd->carry_height);
    printf("GOTO (%.3f, %.3f)\n", cmd->x_ref[1], cmd->y_ref[1]);
    printf("Disable magnet\n");
    printf("#### END COMMAND ####\n");
    
    
    mq_send(to_x, (char *) &(cmd->x_ref[0]), sizeof(cmd->x_ref[0]), 0);
    mq_receive(from_x, NULL, BUFFER_SIZE, 0);
    printf("[C] Moved to x: %.3f\n", cmd->x_ref[0]);
    
    mq_send(to_y, (char *) &(cmd->y_ref[0]), sizeof(cmd->y_ref[0]), 0);
    mq_receive(from_y, NULL, BUFFER_SIZE, 0);
    printf("[C] Moved to y: %.3f\n", cmd->y_ref[0]);
    
    if(cmd->magnet == ENABLE){
#ifndef TEST
      enable_magnet();
      usleep(1000 * 10);
#else
      printf("Magnet enabled\n");
#endif
    }
    mq_send(to_y, (char *) &(cmd->carry_height), sizeof(cmd->carry_height), 0);
    mq_receive(from_y, NULL, BUFFER_SIZE, 0);
    printf("[C] Moved to y: %.3f\n", cmd->carry_height);

    mq_send(to_x, (char *) &(cmd->x_ref[1]), sizeof(cmd->x_ref[1]), 0);
    mq_receive(from_x, NULL, BUFFER_SIZE, 0);
    printf("[C] Moved to x: %.3f\n", cmd->x_ref[1]);

    mq_send(to_y, (char *) &(cmd->y_ref[1]), sizeof(cmd->y_ref[1]), 0);
    mq_receive(from_y, NULL, BUFFER_SIZE, 0);
    printf("[C] Moved to y: %.3f\n", cmd->y_ref[1]);

    if(cmd->magnet == ENABLE){
#ifndef TEST
      disable_magnet();
#else
      printf("Magnet disabled\n");
#endif
    }
    mq_send(to_y, (char *) &reset_pos_flag, sizeof(reset_pos_flag), 0);
    mq_receive(from_y, NULL, BUFFER_SIZE, 0);

    mq_send(to_x, (char *) &reset_pos_flag, sizeof(reset_pos_flag), 0);
    mq_receive(from_x, NULL, BUFFER_SIZE, 0);
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
  attr.mq_msgsize = BUFFER_SIZE;  
  attr.mq_curmsgs = 0;  

  if ( mq_open(Q_TO_X, O_RDONLY | O_CREAT, 0664, &attr) == -1 ||
       mq_open(Q_TO_Y, O_WRONLY | O_CREAT, 0664, &attr) == -1 ||
       mq_open(Q_TO_C, O_WRONLY | O_CREAT, 0664, &attr) == -1 ||
       mq_open(Q_FROM_X, O_RDONLY | O_CREAT, 0664, &attr) == -1 ||
       mq_open(Q_FROM_Y, O_WRONLY | O_CREAT, 0664, &attr) == -1 ||
       mq_open(Q_FROM_C, O_WRONLY | O_CREAT, 0664, &attr) == -1
     ){
      printf("ERROR: %s\n", strerror(errno));
      return -1;
  }
  
  pthread_create(&thread_xcontroller, NULL, rt_x_axies_controller, NULL);
  pthread_create(&thread_ycontroller, NULL, rt_y_axies_controller, NULL);
  pthread_create(&thread_controller, NULL, controller, NULL);
  return 0;
}

int main(int argc,char* argv[]){  
  if( init() == -1)
    exit(-1);
  
  mqd_t to_c, from_c;
  to_c = mq_open(Q_TO_C, O_WRONLY);
  from_c = mq_open(Q_FROM_C, O_RDONLY);

  double tmp;
  float a;
  crane_cmd_t cmd;
    
  while(1) {
    printf ("Enter a crane command <x1,y1 x2,y2 {1|0}>:\n");
    scanf("%lf,%lf %lf,%lf %d",
	  &(cmd.x_ref[0]), &(cmd.y_ref[0]), &(cmd.x_ref[1]), &(cmd.y_ref[1]), &(cmd.magnet) );

    mq_send(to_c, (char *) &cmd, sizeof(cmd), 0);
    mq_receive(from_c, NULL, BUFFER_SIZE, 0);
  }  
  
}
