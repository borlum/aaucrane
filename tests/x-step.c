#include <stdio.h>
#include <stdlib.h>
#include <comedilib.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>
#include <limits.h>

const int range = 0;
const int aref = AREF_GROUND;

const char device_name[] = "/dev/comedi0";
const char data_file[] = "x-step.csv";

int main(int argc, char *argv){
  comedi_t *device = comedi_open(device_name);
  if(device == NULL){
    printf("Error opening file, %s\n", filename);
    exit(1);
  }  
  comedi_data_write(device, 1, 0, range, aref, motor_out);
}

int dump_sensor_data(){
  FILE *fp = fopen(data_file, "w");

  lsampl_t data, maxdata;
  comedi_range *range_info;
  double physical_value;
  int i;

  if(fp == NULL){
    printf("Could not create file %s\n", data_file);
  }

  fprintf(fp, "ANGLE,XPOS,YPOS,XTACHO,YTACHO\n");

  while(1){
    for(i = 0; i < 5; i++){
      comedi_data_read(device, 0, i, range, aref, &data);
      comedi_set_global_oor_behavior(COMEDI_OOR_NAN);
      range_info = comedi_get_range(device, subdev, i, range);
      maxdata = comedi_get_maxdata(device, subdev, i);
      physical_value= comedi_to_phys(data, range_info, maxdata);
      fprintf(fp, "%g,", physical_value);
      printf("Data from sensor %d: %g\n", i, physical_value);
    }
    fprintf(fp, "\n");
    usleep(250 * 1000);
  }
}

