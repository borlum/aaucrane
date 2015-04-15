#include <unistd.h>
#include <stdlib.h>
#include <comedilib.h>

#define AIN_SUBDEV 0
#define AOUT_SUBDEV 1
#define DIO_SUBDEV 2
#define DEVICE "/dev/comedi0"

#define CHAN_MAGNET_OUT 7
#define CHAN_MAGNET_BTN 3

#define CHAN_XMOTOR_OUT 0
#define CHAN_YMOTOR_OUT 1

#define CHAN_ANGLE_IN 12
#define CHAN_ANGLE_OLD_IN 0

#define CHAN_XPOS_IN 13
#define CHAN_YPOS_IN 2

#define CHAN_XVEL_IN 11
#define CHAN_YVEL_IN 4

#define CHAN_XIN_IN 9
#define CHAN_YIN_IN 10

#define CHAN_CTRLPAD_X_IN 14
#define CHAN_CTRLPAD_Y_IN 15

comedi_t *NI_card;


double get_sensor_raw(int channel)
{
    double physical_value = -1;
    
    #ifndef TESTING
    lsampl_t data, max_data;
    comedi_range *range_info;

    comedi_data_read(NI_card, AIN_SUBDEV, channel, 0, AREF_GROUND, &data);
    
    range_info     = comedi_get_range(NI_card, AIN_SUBDEV, channel, 0);
    max_data       = comedi_get_maxdata(NI_card, AIN_SUBDEV, channel);
    physical_value = comedi_to_phys(data, range_info, max_data);

    return physical_value;
    #endif

    return physical_value;
}

int main(){
  int tmp;
  NI_card = comedi_open(DEVICE);
  /*Global config*/
  comedi_set_global_oor_behavior(COMEDI_OOR_NUMBER);
  comedi_dio_config(NI_card, DIO_SUBDEV, CHAN_MAGNET_OUT, COMEDI_OUTPUT);
  comedi_dio_config(NI_card, DIO_SUBDEV, CHAN_MAGNET_BTN, COMEDI_INPUT);

  while(1){
    while(scanf("Comedi value:%d", &tmp));
    comedi_data_write(NI_card, AOUT_SUBDEV, 0, 0, AREF_GROUND, tmp);
    usleep(1000);
    printf("%d,%f\n", tmp, (get_sensor_raw(CHAN_XIN_IN) * 2));
  }
  
  for(int output = 0; output <= 4200; output += 200){
    comedi_data_write(NI_card, AOUT_SUBDEV, 0, 0, AREF_GROUND, output);
    usleep(1000);
    printf("%d,%f\n", output, (get_sensor_raw(CHAN_XIN_IN) * 2));
  }
}
