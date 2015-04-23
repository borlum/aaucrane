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



pthread_t t_xcontroller = NULL, t_ycontroller = NULL, t_logger = NULL;

#ifdef RTAI
static RT_TASK *rt_xcontroller, *rt_ycontroller, *rt_logger;
#endif

void* logger(void* args){
  FILE* fp;
  unsigned long t_0, t_sample;

#ifdef RTAI
  RTIME period = nano2count(1000 * 1000 * 1); /* We think 10 milli */
  if(!(rt_logger = rt_task_init_schmod(nam2num("logger"), 1, 0, 0, SCHED_FIFO, 0))){
    printf("Could not start rt_task\n");
    exit(42);
  }
  rt_task_make_periodic(rt_logger, rt_get_time() + period, period);
  rt_make_hard_real_time();
#endif

  char tmp[160];
  sprintf(tmp, "%s/%d.csv", DATA_PATH, (int)time(NULL));
  fp = fopen(tmp, "w");
  fprintf(fp, DATA_HEADER);

  t_0 = get_time_micros();
  while(1){
    /*GRAB TIMESTAMP*/
    t_sample = get_time_micros();
    fprintf(fp, "%ld,",  (t_sample - t_0));

    /*SAMPLE SENSORS*/
    fprintf(fp, "%f,", get_old_angle_raw());
    fprintf(fp, "%f,", get_angle_raw());
    fprintf(fp, "%f,", get_xpos_raw());
    fprintf(fp, "%f,", get_ypos_raw());
    fprintf(fp, "%f,", get_motorx_velocity_raw());
    fprintf(fp, "%f,", get_motory_velocity_raw());
    fprintf(fp, "%f,", get_motorx_voltage());
    fprintf(fp, "%f", get_motory_voltage());
    fprintf(fp, "\n");
#ifdef RTAI
    rt_task_wait_period();
#else
    usleep(1000);
#endif
  }
}

int init(){
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
  return 0;
}


int main(int argc,char* argv[]){
  if( init() == -1)
    exit(-1);

  mqd_t to_x, from_x;
  to_x = mq_open(Q_TO_X, O_WRONLY);
  from_x = mq_open(Q_FROM_X, O_RDONLY);

  double x;
  double y = 0.223;

  size_t len = 2 * BUFFER_SIZE;
  char stupid_buffer[2 * BUFFER_SIZE];
  double tmp;
  
  while(1) {
    printf ("Enter a step size: <x>:\n");
    scanf("%lf", &x);

    if(t_xcontroller == NULL && t_logger == NULL){
      printf("Resetting ... .. .\n");
      pthread_create(&t_xcontroller, NULL, task_x_axies_controller, NULL);
      /*pthread_create(&t_ycontroller, NULL, rt_y_axies_controller, NULL);*/
      //usleep(1000 * 1000);
      printf("starting logger\n");
      pthread_create(&t_logger, NULL, logger, NULL);

    }

    if(mq_send(to_x, (char *) &x, sizeof(x), 0) == -1)
      printf("ERROR: send: %s\n", strerror(errno));

    if(mq_receive(from_x, stupid_buffer, sizeof(double), 0) == -1)
      printf("ERROR: recv: %s\n", strerror(errno));
    else{
      tmp =  (double) *stupid_buffer;
      printf("Read: %lf", tmp);
    }
    printf("Done\n");
  }
}
