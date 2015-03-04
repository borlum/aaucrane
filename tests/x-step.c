#include <stdio.h>
#include <stdlib.h>
#include <comedilib.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>
#include <limits.h>
#include <sys/time.h>

const int range = 0;
const int aref = AREF_GROUND;

const char device_name[] = "/dev/comedi0";
const char data_file[] = "x-step.csv";

int dump_sensor_data(void);

comedi_t *device;

int main(int argc, char *argv){
  device = comedi_open(device_name);
  if(device == NULL){
    printf("Error opening file, %s\n", device_name);
    exit(1);
  }
  dump_sensor_data();
}

int dump_sensor_data(){
  FILE *fp = fopen(data_file, "w");
  struct timeval ts;
  int sensors[] = {0, 1, 2, 3, 4, 9, 10};
  int len = sizeof(sensors) / sizeof(int);
  int sampl_nr = 0;


  /*Comedi vars */
  lsampl_t data, maxdata;
  comedi_range *range_info;
  double physical_value;

  if(fp == NULL){
    printf("Could not create file %s\n", data_file);
    exit(1);
  }

  fprintf(fp, "ANGLE,XPOS,YPOS,XTACHO,YTACHO,XVOLT,YVOLT,TIMESTAMP\n");

  while(1){
    
    if(sampl_nr++ == 500){
        comedi_data_write(device, 1, 0, range, aref, UINT_MAX);
	printf("Starting motor");
    }
    
    gettimeofday(&ts, NULL); /* UTC */
    
    for(int i = 0; i < len; i++){
      comedi_data_read(device, 0, sensors[i], range, aref, &data);
      comedi_set_global_oor_behavior(COMEDI_OOR_NAN);
      range_info = comedi_get_range(device, 0, sensors[i], range);
      maxdata = comedi_get_maxdata(device, 0, sensors[i]);
      physical_value= comedi_to_phys(data, range_info, maxdata);
      fprintf(fp, "%g,", physical_value);
      //printf("Data from sensor %d: %g\n", sensors[i], physical_value);
    }

    fprintf(fp, "%lu, %lu", ts.tv_sec, ts.tv_usec);
    
    fprintf(fp, "\n");
    usleep(250 * 1000);
  }
}

