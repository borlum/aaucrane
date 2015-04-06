#include <libcrane.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
  initialize_crane();
  
  while(1){
#ifdef ANGLE
    printf("Raw measurement: %f | Converted to rad: %f | Converted to deg: %f\n", get_angle_raw(), get_angle(), get_angle() * (180.0/3.14));
#else
    printf("JOYPAD: (x,y) = (%3f, %3f)\n", get_ctrlpad_x(), get_ctrlpad_y());
    printf("X_pos: %3f @ %3f| Y_pos: %3f @ %3f | ANGLE: %3f\n", get_xpos(), get_xpos_raw(), get_ypos(), get_ypos_raw(), get_angle());
    run_motorx(get_ctrlpad_x());
    run_motory(get_ctrlpad_y());

    if (get_ctrlpad_magnet_switch()) {
        enable_magnet();
    } else {
        disable_magnet();
    }
#endif
    usleep(1000 * 100);
  }
}
