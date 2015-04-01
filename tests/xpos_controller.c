#include <stdlib.h>
#include <libcrane.h>

static double K_theta = 12;
static double K_p = 26.6;

double notch_filter(double theta);

int main(int argc,char* argv[]){

  run_motorx(0);
  run_motory(0);
  
  if(argc != 2){
    printf("usage: %s <distance in meters>", argv[0]);
    exit(1);
  }
  
  double x_ref = atof(argv[1]);
  double x_err = 0;

  double output = -1;

  printf("Moving to x = %f\n", x_ref);

  while(output != 0){
    output = (x_ref - get_xpos()) * K_p;

    if(output > 14)
      output = 14;
    else if (output < -14)
      output = -14;

    run_motorx(output);
  }  
  
  printf("Jobs done\n");

}
