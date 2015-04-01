#include <libcrane.h>

static double K_theta = 12;
static double K_p = 26.6;

double notch_filter(double theta);

int main(int argc,char* argv[]){

  run_motorx(0);
  run_motory(0);
  
  if(argc != 2){
    printf("usage: %s <distance in meters>", argv[0]);
  }

  for(int i = 0; i < argc; i++){
    ptinf("%c", argv[i]);
  }
  
  double angle_ref = 0;
  double angle_err = 0;

  double x_ref = atof(argv[1]);
  double x_err = 0;

  double output = -1;

  printf("%f", x_ref);
  
  while(output != 0){
    angle_err = (angle_ref - get_angle()) * K_theta;

    output = notch_filter(angle_err);

    output = (output + x_ref - get_xpos()) * K_p;

    if(output > 14)
      output = 14;
    else if (output < -14)
      output = -14;

    run_motorx(output);
  }  
}

double notch_filter(double theta){
  return theta;
}
