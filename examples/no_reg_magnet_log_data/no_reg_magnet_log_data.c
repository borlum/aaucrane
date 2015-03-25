#include <stdio.h>
#include <stdlib.h>
#include <comedilib.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>

#define DIGITAL_IO_SUBDEV 2
#define MAGNET_FLIP 3
//48 = 7
#define MAGNET_ENABLE 7

int run = 1;

int range = 0;
int aref = AREF_GROUND;
comedi_t *device;

const char filename[] = "/dev/comedi0";

void signal_handler(int signal){
  if(signal == SIGINT)
    run = 0;
}

int read_and_write(int in_channel, int out_channel) {
  lsampl_t data;
  double physical_value;
  int retval;
  comedi_range * range_info;
  lsampl_t maxdata;
  unsigned int motorX_out = 2048;

  retval = comedi_data_read(device, 0, in_channel, range, aref, &data);
  if(retval < 0)
  {
    comedi_perror(filename);
    return -1;
  }

  comedi_set_global_oor_behavior(COMEDI_OOR_NAN);
  range_info     = comedi_get_range(device, 0, in_channel, range);
  maxdata        = comedi_get_maxdata(device, 0, in_channel);
  physical_value = comedi_to_phys(data, range_info, maxdata);
  motorX_out     = (((data-2047) * 3000) / 2047) + 500;
  comedi_data_write(device, 1, out_channel, range, aref, motorX_out);
}

void control_magnet() {
  lsampl_t IN;
  /*52, 17, 49, 47, 19*/
  /*TRYK: 17, 52*/
  /*FLIP: 47, 49*/

  //IO SUBDEV: 10, 7, 2
  comedi_dio_read(device, DIGITAL_IO_SUBDEV, MAGNET_FLIP, &IN);
  comedi_dio_write(device, DIGITAL_IO_SUBDEV, MAGNET_ENABLE, IN);
}

void save_data(FILE *fp, int *sensors, int num_sensors){
  for (int i = 0; i < num_sensors; i++) {
    comedi_data_read(device, 0, sensors[i], range, aref, &data);
    comedi_set_global_oor_behavior(COMEDI_OOR_NAN);
    range_info     = comedi_get_range(device,   0, sensors[i], range);
    maxdata        = comedi_get_maxdata(device, 0, sensors[i]);
    physical_value = comedi_to_phys(data, range_info, maxdata);
    fprintf(fp, "%g,", physical_value);
  }
  fprintf(fp, "\n");
}

int main(int argc, char *argv[])
{
  int sensors[] = {0, 12, 13, 2, 3, 4, 9, 10}; /* TIMESTAMP,ANGLE1,ANGLE2,XPOS,YPOS,XTACHO,YTACHO,XVOLT,YVOLT */
  int num_sensors = sizeof(sensors) / sizeof(int);
  char data_filename[100];
  FILE *fp;
  sptinf(data_filename, "/var/www/html/data/crane/xsteps/MANUAL_%d.csv", (int)time(NULL));
  device = comedi_open(filename);
  if(device == NULL)
  {
    comedi_perror(filename);
    return -1;
  }

  signal(SIGINT, signal_handler);
  
  fp = fopen(data_filename, "w");
  fprintf(fp, "TIMESTAMP,ANGLE1,ANGLE2,XPOS,YPOS,XTACHO,YTACHO,XVOLT,YVOLT\n");

  comedi_dio_config(device, DIGITAL_IO_SUBDEV, MAGNET_FLIP, COMEDI_INPUT);
  comedi_dio_config(device, DIGITAL_IO_SUBDEV, MAGNET_ENABLE, COMEDI_OUTPUT);
  while(run)
  {

    read_and_write(14, 0);
    read_and_write(15, 1);
    control_magnet();
    save_data(fp, sensors, num_sensors);
    usleep(1000); /*ms -> us*/
  }
  return -1;
}
