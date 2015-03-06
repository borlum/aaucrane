#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>
#include <limits.h>
#include <sys/time.h>
#include <time.h>

#define ON_CRANE 2

#ifdef ON_CRANE
#include <comedilib.h>
const int range = 0;
const int aref = AREF_GROUND;
const char device_name[] = "/dev/comedi0";
comedi_t *device;
#endif

int t1, t2;

const char data_file[] = "x-step.csv";

int timeNowUsec(){
    //Struct for holding elapsed time!
    struct timeval now;

    //Tager pointers til en timeval struct og en timezone struct
    //Og sætter i timeval tiden i sekunder og microsekunder siden EPOCH!
    //Lad den sidste parameter være NULL, den skal vi ikke bruge til noget.
    gettimeofday(&now, NULL);

    //Returner fra vores timeval struct, "now", tiden kun i microsekunder, siden EPOCH!
    return (now.tv_sec) * 1000000 + (now.tv_usec);
}

int dump_sensor_data(void);

int main(int argc, char *argv){
  #ifdef ON_CRANE
  device = comedi_open(device_name);
  if (device == NULL) {
    printf("Error opening file, %s\n", device_name);
    exit(1);
  }
  #endif
  dump_sensor_data();
}

int dump_sensor_data(){
  FILE *fp;
  struct timeval ts;
  int sensors[] = {0, 1, 2, 3, 4, 9, 10};
  int len = sizeof(sensors) / sizeof(int);
  int sampl_nr = 0;

  #ifdef ON_CRANE
  /*Comedi vars */
  lsampl_t data, maxdata;
  comedi_range *range_info;
  double physical_value;
  #endif

  fp = fopen(data_file, "w");
  fprintf(fp, "ANGLE,XPOS,YPOS,XTACHO,YTACHO,XVOLT,YVOLT,TIMESTAMP\n");
  fclose(fp);

  t1 = timeNowUsec();

  while(1){
    fp = fopen(data_file, "a");
    if (fp == NULL) {
      printf("Could not create file %s\n", data_file);
      exit(1);
    }

    if (sampl_nr == 10) {
        #ifdef ON_CRANE
        comedi_data_write(device, 1, 0, range, aref, 0);
        #endif
	      printf("Starting motor\n");
    }
    
    for (int i = 0; i < len; i++) {
      #ifdef ON_CRANE
      comedi_data_read(device, 0, sensors[i], range, aref, &data);
      comedi_set_global_oor_behavior(COMEDI_OOR_NAN);
      range_info = comedi_get_range(device, 0, sensors[i], range);
      maxdata = comedi_get_maxdata(device, 0, sensors[i]);
      physical_value= comedi_to_phys(data, range_info, maxdata);
      fprintf(fp, "%g,", physical_value);
      printf("Data from sensor %d: %g\n", sensors[i], physical_value);
      #else
      fprintf(fp, "%g,", 42.0);
      printf("Data from sensor %d: %g\n", sensors[i], 42.0);
      #endif
    }

    t2 = timeNowUsec();

    int diff = t2 - t1;

    fprintf(fp, "%d", diff);
    fprintf(fp, "\n");
    fclose(fp);
    usleep(10 * 1000);
    sampl_nr++;
  }
}