#include <libcrane.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    initialize_crane();
    
    while(1) {
        printf("(x,y,\u03B8) = (%.3lf, %.2lf, %.2lf)\n", get_xpos(), get_ypos(), get_angle());
        usleep(1000000 * .1);
    }
}