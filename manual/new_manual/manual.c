#include <libcrane.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
  initialize_crane();
  
  while(1){
    printf("JOYPAD: (x,y) = (%3f, %3f)", get_ctrlpad_x(), get_ctrlpad_y());
    printf("X_pos: %3f | Y_pos: %3f | ANGLE: ", get_xpos(), get_ypos(), get_angle());
    run_motorx(get_ctrlpad_x());
    run_motory(get_ctrlpad_y());
    usleep(1000 * 100);
  }
}
