#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <pthread.h>
#include <mqueue.h>
#include <fcntl.h>

#include <rtai_lxrt.h>

#include <comedilib.h>
#include <libcrane.h>
#include "acc.h"
#include "controller.h"

#define DATA_PATH "/var/www/html/data/acc/steps/"
#define DATA_HEADER "TIME,ANGLE1,ANGLE2,XPOS,YPOS,XTACHO,YTACHO,XVOLT,YVOLT\n"
#define NAME_LEN 160

pthread_t t_xcontroller = NULL, t_ycontroller = NULL, t_logger = NULL;

#ifdef RTAI
static RT_TASK *rt_xcontroller, *rt_ycontroller, *rt_logger;
#endif

int init(){
#ifdef RTAI
  if(rt_is_hard_timer_running() == 1){
    printf("Timer is running...\n");
  }
  else{
    printf("Starting timer...\n");
    rt_set_periodic_mode();
    start_rt_timer(0);
  }
#endif /* RTAI */
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

  if ( mq_open(Q_TO_X, O_RDONLY   | O_CREAT, 0664, &attr) == -1 ||
       mq_open(Q_TO_Y, O_WRONLY   | O_CREAT, 0664, &attr) == -1 ||
       mq_open(Q_TO_C, O_WRONLY   | O_CREAT, 0664, &attr) == -1 ||
       mq_open(Q_FROM_X, O_RDONLY | O_CREAT, 0664, &attr) == -1 ||
       mq_open(Q_FROM_Y, O_WRONLY | O_CREAT, 0664, &attr) == -1 ||
       mq_open(Q_FROM_C, O_WRONLY | O_CREAT, 0664, &attr) == -1
  ){
    printf("ERROR: %s\n", strerror(errno));
    return -1;
  }
  return 0;
}


int main(int argc,char* argv[]){

  if( init() == -1)
    exit(-1);

  init_logger("/var/www/html/data/acc/steps/", sizeof("/var/www/html/data/acc/steps/"));

  mqd_t to_x, from_x, to_y, from_y;
  to_x = mq_open(Q_TO_X, O_WRONLY);
  from_x = mq_open(Q_FROM_X, O_RDONLY);

  to_y = mq_open(Q_TO_Y, O_WRONLY);
  from_y = mq_open(Q_FROM_Y, O_RDONLY);

  double y;
  
  size_t len = 2 * BUFFER_SIZE;
  char stupid_buffer[2 * BUFFER_SIZE];
  double tmp;

  while(1) {
    printf ("Enter a step size: <y>:\n");
    scanf("%lf", &y);
    
    if (t_xcontroller == NULL && t_logger == NULL) {
      pthread_create(&t_ycontroller, NULL, task_y_axis_controller, NULL);
      pthread_create(&t_logger, NULL, task_logger, NULL);
    }

    enable_logger();

    /*Disable for NO CONTAINER*/
    disable_magnet();
    
    if (mq_send(to_y, (char *) &y, sizeof(y), 0) == -1)
      printf("ERROR: send: %s\n", strerror(errno));
    if (mq_receive(from_y, stupid_buffer, len, 0) == -1)
      printf("ERROR: recv: %s\n", strerror(errno));
    else {
      memcpy(&tmp, stupid_buffer, sizeof(int));
      printf("Read: %lf", tmp);
    }

    disable_logger();

    printf("Done!\n");
  }
}
