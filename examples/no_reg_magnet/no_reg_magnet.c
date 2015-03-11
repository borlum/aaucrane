#include <stdio.h>
#include <stdlib.h>
#include <comedilib.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>

int range = 0;
int aref = AREF_GROUND;
comedi_t *device;

const char filename[] = "/dev/comedi0";

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
  comedi_dio_config(device, 2, 9, COMEDI_INPUT);
  if (comedi_data_read(device, 2, 9, 0, AREF_GROUND, &IN) == -1)
  {
    printf("ÅHHH NEJ! 1\n");
  }
  else
  {
    printf("BUTTON READ: %d\n", IN);
  }
  //comedi_dio_write(device, 1, 7, IN);
}

int main(int argc, char *argv[])
{
  device = comedi_open(filename);
  if(device == NULL)
  {
    comedi_perror(filename);
    return -1;
  }

  while(1)
  {

    //read_and_write(14, 0);
    //read_and_write(15, 1);
    control_magnet();
    usleep(250 * 1000); /*ms -> us*/
  }
  return -1;
}