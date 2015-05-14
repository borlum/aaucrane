#include <libcrane.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    initialize_crane();
    
    while(1) {
    printf("(x,y,\u03B8 uden,\u03B8 med) = (%+.3lf, %+.3lf, %+.3lf, %+.3lf)\n", get_xpos(), get_ypos(), get_angle(), get_angle() + 0.021);
    }
}
