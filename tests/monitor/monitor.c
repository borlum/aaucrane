#include <libcrane.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    initialize_crane();
    
    while(1) {

        if (get_ctrlpad_magnet_switch()) {
            enable_magnet();
        } else {
            disable_magnet();
        }
        
        printf("(x,y,\u03B8) = (%.3lf, %.3lf, %.3lf)\n", get_xpos(), get_ypos(), get_angle());
        usleep(1000000 * .1);
    }
}