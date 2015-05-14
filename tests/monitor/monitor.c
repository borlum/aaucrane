#include <libcrane.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    initialize_crane();
    
    while(1) {
    printf("\033[2J");

    printf("x: %+.3lf [m] | y: %+.3lf [m] | \u03B8 (u): %+.3lf [rad] | \u03B8 (m): %+.3lf [rad] | U: %+.3lf [V]\n", get_xpos(), get_ypos(), get_angle(), get_angle() + 0.021, get_motorx_voltage());
}
