#include <stdlib.h>
#include <libcrane.h>

static double K_theta = 12;
static double K_p = 26.6;

double notch_filter(double theta);

int main(int argc,char* argv[]){  
  if(argc != 2){
    printf("usage: %s <distance in meters>", argv[0]);
    exit(1);
  }

  initialize_crane();  
  run_motorx(0);
  run_motory(0);

  
  double x_ref = atof(argv[1]);
  double x_err = 0;

  double output = -1;

  printf("Moving to x = %f\n", x_ref);

  while(output != 0){
    x_err = get_xpos();
    output = (x_ref - x_err) * K_p;
    printf("X_ref: %3f | X_err: %3f | Output: %3f\n", x_ref, x_err, output);

    
    if(output > 14)
      output = 14;
    else if (output < -14)
      output = -14;

    printf("OUTPUT: %f\n", (flaot) output);
    
    printf("ret val %d\n", run_motorx( (int) output));

    usleep(1000 * 500);
  }  
  
  printf("Jobs done\n");

}
