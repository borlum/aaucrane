#include <pthread.h>
#include <mqueue.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stirng.h>

#include <comedilib.h>
#include <libcrane.h>
#include "acc.h"
#include "controller.h"


#define DATA_PATH "/var/www/html/data/crane/xsteps/"
#define DATA_HEADER "TIME,ANGLE1,ANGLE2,XPOS,YPOS,XTACHO,YTACHO,XVOLT,YVOLT\n"



pthread_t t_xcontroller, t_ycontroller, t_logger;

void* logger(void* args){
  FILE* fp;

  unsigned long t_0, t_sample;
  
  char tmp[160];
  sprintf(tmp, "%s/%d.csv", DATA_PATH, (int)time(NULL));
  fp = fopen(tmp, "w");
  fprintf(fp, DATA_HEADER);  
  
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
    
    usleep(1000 * 1); /* Log every 1 ms */
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
  return 0;
}


int main(int argc,char* argv[]){  
  if( init() == -1)
    exit(-1);
  
  mqd_t to_x, to_y;
  to_x = mq_open(Q_TO_X, O_WRONLY);
  to_y = mq_open(Q_TO_X, O_RDONLY);

  double x;
  double y;
    
  while(1) {
    printf ("Enter a coodinta set <x,y>:\n");
    scanf("%lf, %lf", &x, &y);

    printf("Resetting ... .. .\n");
    pthread_create(&t_xcontroller, NULL, rt_x_axies_controller, NULL);
    pthread_create(&t_ycontroller, NULL, rt_y_axies_controller, NULL);
    usleep(1000 * 1000);
    printf("starting logger\n");
    pthread_create(&t_logger, NULL, logger, NULL);

    mq_send(to_x, (char *) &x, sizeof(x), 0);
    mq_send(to_y, (char *) &y, sizeof(y), 0);
  } 
}
