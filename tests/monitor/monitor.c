#include <libcrane.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    initialize_crane();
    
    while(1) {
        printf("(x,y,\u03B8) = (%.3lf, %.3lf, %.3lf)\n", get_xpos(), get_ypos(), get_angle());
        printf("ANGLE RAW VOLTAGE = %lf \n", get_angle_raw());
        usleep(1000000 * .1);
    }
}