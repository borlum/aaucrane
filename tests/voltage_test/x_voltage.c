#include <unistd.h>
#include <stdlib.h>
#include <comedi.h>

#define CHAN_XIN_IN 9

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
  comedi_t *NI_card;

  NI_card = comedi_open(DEVICE);
  /*Global config*/
  comedi_set_global_oor_behavior(COMEDI_OOR_NUMBER);
  comedi_dio_config(NI_card, DIO_SUBDEV, CHAN_MAGNET_OUT, COMEDI_OUTPUT);
  comedi_dio_config(NI_card, DIO_SUBDEV, CHAN_MAGNET_BTN, COMEDI_INPUT);

  for(int output = 0; output <= 4000; output += 200){
    comedi_data_write(NI_card, AOUT_SUBDEV, 0, 0, AREF_GROUND, output);
    printf("%d,%f\n", output, (get_sensor_raw(CHAN_XIN_IN) * 2));
  }
}
