#include <libcrane.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    initialize_crane();
    
    while(1) {
    printf("\033[2J");

    printf("x: %+.3lf | y: %+.3lf | \u03B8 (u): %+.3lf | \u03B8 (m): %+.3lf | U: %+.3lf \n", get_xpos(), get_ypos(), get_angle(), get_angle() + 0.021, get_motorx_voltage());
}
