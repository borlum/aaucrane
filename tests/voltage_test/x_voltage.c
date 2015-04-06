#include <libcrane.h>
#include <unistd.h>
#include <stdlib.h>

int main(){

  initialize_crane();
  for(int output = 0; output <= 4000; output += 200){
    comedi_data_write(NI_card, AOUT_SUBDEV, 0, 0, AREF_GROUND, output);
    printf("%d,%f\n", output, get_motorx_voltage());
  }
}
