#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>

#include <pthread.h>
#include <fcntl.h>
#include <mqueue.h>
#include <string.h>
#include <stdio.h>

#ifdef RTAI
#include <rtai_lxrt.h>
#endif

#include <libcrane.h>
#include "acc.h"
#include "include/controller.h"
#include "include/stack.h"

/* Threads */
pthread_t thread_xcontroller;
pthread_t thread_ycontroller;
pthread_t thread_controller;
pthread_t thread_logger;

void *controller(void * args)
{
  setbuf(stdout, NULL);
  printf("Starting controller task\n");
  mqd_t to_x, from_x;
  mqd_t to_y, from_y;
  mqd_t to_c, from_c;

  size_t buf_len = BUFFER_SIZE;
  char buf[BUFFER_SIZE];  

  crane_cmd_t* cmd;
  size_t cmd_buf_len = BUFFER_SIZE;
  char cmd_buf[BUFFER_SIZE];

  int tmp = 0;

  to_x = mq_open(Q_TO_X, O_WRONLY);
  from_x = mq_open(Q_FROM_X, O_RDONLY);

  to_y = mq_open(Q_TO_Y, O_WRONLY);
  from_y = mq_open(Q_FROM_Y, O_RDONLY);
  
  to_c = mq_open(Q_TO_C, O_RDONLY);
  from_c = mq_open(Q_FROM_C, O_WRONLY);

  /* Wait for the crane is @ 0,0 */
  pthread_create(&thread_xcontroller, NULL, task_x_axis_controller, NULL); /* Starts with a ref @ 0 */
  pthread_create(&thread_ycontroller, NULL, task_y_axis_controller, NULL);  /* Starts with a ref @ 0 */
  /* Wait untill (0,0) has been reached */
  mq_send(to_y, (char*) &tmp, sizeof(double), 0);
  mq_receive(from_y, buf, buf_len, 0);
  mq_send(to_x, (char*) &tmp, sizeof(double), 0);
  mq_receive(from_x, buf, buf_len, 0);
  mq_send(from_c, (char *) &cmd, sizeof(cmd), 0);

  while(1){
    printf("Crane reday for next command... \n");
    mq_receive(to_c, cmd_buf, cmd_buf_len, 0);
    cmd = (crane_cmd_t*) cmd_buf;
    
    printf("#### NEW COMMAND ####\n");
    printf("GOTO (%.3f, %.3f)\n", cmd->pickup_point.x, cmd->pickup_point.y);
    printf("Carry @ %.3f\n", cmd->carry_height);
    printf("GOTO (%.3f, %.3f)\n", cmd->dest_point.x, cmd->dest_point.y);
    printf("#### END COMMAND ####\n");    
    
    mq_send(to_x, (char *) &(cmd->pickup_point.x), sizeof(cmd->pickup_point.x), 0);
    mq_receive(from_x, buf, buf_len, 0);
    printf("[C] Moved to x: %.3f\n", cmd->pickup_point.x);
    
    mq_send(to_y, (char *) &(cmd->pickup_point.y), sizeof(cmd->pickup_point.y), 0);
    mq_receive(from_y, buf, buf_len, 0);
    printf("[C] Moved to y: %.3f\n", cmd->pickup_point.y);

    enable_magnet();
    usleep(1000 * 100);

    mq_send(to_y, (char *) &(cmd->carry_height), sizeof(cmd->carry_height), 0);
    mq_receive(from_y, buf, buf_len, 0);
    printf("[C] Moved to y: %.3f\n", cmd->carry_height);

    mq_send(to_x, (char *) &(cmd->dest_point.x), sizeof(cmd->dest_point.x), 0);
    mq_receive(from_x, buf, buf_len, 0);
    printf("[C] Moved to x: %.3f\n", cmd->dest_point.x);

    mq_send(to_y, (char *) &(cmd->dest_point.y), sizeof(cmd->dest_point.y), 0);
    mq_receive(from_y, buf, buf_len, 0);
    printf("[C] Moved to y: %.3f\n", cmd->dest_point.y);
    
    disable_magnet();

    mq_send(to_y, (char *) &(cmd->carry_height), sizeof(cmd->carry_height), 0);
    mq_receive(from_y, buf, buf_len, 0);
    mq_send(from_c, (char *) &cmd, sizeof(cmd), 0);
  }
}

int init(){
  setbuf(stdout, NULL);
#ifndef TEST
  initialize_crane();  
  run_motorx(0);
  run_motory(0);
#endif /* TEST */
#ifdef RTAI
  if(rt_is_hard_timer_running() == 1){
    printf("Timer is running");
  }
  else{
    printf("Starting timer \n");
    //rt_set_oneshot_mode(); /* ONE SHOT! */
    rt_set_periodic_mode();
    start_rt_timer(0);
  }
#endif /* RTAI */
  
  struct mq_attr attr;  
  attr.mq_flags = 0;  
  attr.mq_maxmsg = 10;  
  attr.mq_msgsize = BUFFER_SIZE;  
  attr.mq_curmsgs = 0;  

  if ( mq_unlink(Q_TO_X) == -1 ||
       mq_unlink(Q_TO_Y) == -1 ||
       mq_unlink(Q_TO_C) == -1 ||
       mq_unlink(Q_FROM_X) == -1 ||
       mq_unlink(Q_FROM_Y) == -1 ||
       mq_unlink(Q_FROM_C) == -1
       ){
    printf("ERROR: %s\n", strerror(errno));
    return -1;
  }
  
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
  
  pthread_create(&thread_controller, NULL, controller, NULL);
  pthread_create(&thread_logger, NULL, task_logger, NULL);

  return 0;
}

void place_containers(){
  for(int i = 0; i <=3; i++)
    update_status(i, 0, STACK_OCCUPIED);
}

int main(int argc,char* argv[]){  
  init_logger("/var/www/html/data/acc/acc/", sizeof("/var/www/html/data/acc/acc/"));

  if( init() == -1)
    exit(-1);
  mq_receive(from_c, buf, buf_len, 0);

  printf("Before enable_logger\n");
  if (enable_logger() == 0)
    printf("Logger enabled\n");
  else
    printf("Logger failed\n");

  usleep(1000 * 10);

  if (disable_logger() == 0)
    printf("Logger disabled\n");
  else
    printf("Logger failed\n");

  size_t buf_len = BUFFER_SIZE;
  char buf[BUFFER_SIZE];

  init_stack();
  place_containers();
  
  mqd_t to_c, from_c;
  to_c = mq_open(Q_TO_C, O_WRONLY);
  from_c = mq_open(Q_FROM_C, O_RDONLY);

  int source_row, dest_row;
  int source_col, dest_col; 
  
  stack_loc_t source, dest;
  crane_cmd_t cmd;

  /*
  Y_OFF_SET er magnet 1 container over jorden
  Vi vil køre m. magnet i 7 containere over jorden
  => Magnet skal være i: Y_OFF_SET - 6*container højde.
  */
  cmd.carry_height = Y_OFF_SET - (CONTAINER_HEIGHT * 6);
  
  while(1) {
    printf ("Enter a crane command <row,col row,col>:\n");
    scanf("%d,%d %d,%d", &source_row, &source_col, &dest_row, &dest_col);
    if(get_status(source_row, source_col) == STACK_FREE){
      printf("No container found at location %d,%d\n", source_row, source_col);
      continue;
    }
    if(get_status(dest_row, dest_col) == STACK_OCCUPIED){
      printf("Destination is occupied (%d,%d)\n", dest_row, dest_col);
      continue;
    }
    if(get_status(source_row, source_col + 1) == STACK_OCCUPIED){
      printf("Sorry, a container is ontop of source (%d,%d)\n", source_row, source_col);
      continue;
    }
    if(
       source_row > STACK_WIDTH || source_col > STACK_HEIGHT ||
       dest_row > STACK_WIDTH || dest_col > STACK_HEIGHT
       ){
      printf("Sorry, a container is ontop of source (%d,%d)\n", source_row, source_col);
      continue;
    }

    source = get_stack_loc(source_row, source_col);
    dest = get_stack_loc(dest_row, dest_col);

    cmd.pickup_point = source.loc;
    cmd.dest_point = dest.loc;

    if (enable_logger() == 0)
      printf("Logger enabled\n");
    else
      printf("Logger failed\n");
    
    mq_send(to_c, (char *) &cmd, sizeof(cmd), 0);
    update_status(source_row, source_col, STACK_FREE);
    mq_receive(from_c, buf, buf_len, 0);
    update_status(dest_row, dest_col, STACK_OCCUPIED);

    if (disable_logger() == 0)
      printf("Logger disabled\n");
    else
      printf("Logger failed\n");
  } 
}
