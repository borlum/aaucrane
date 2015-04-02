#include <stdlib.h>
#include <unistd.h>
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

  float X[3] = {0}; 
  float E[3] = {0};
  int i = 0;
    
  double x_ref = atof(argv[1]);
  double x_pos = 0;

  double output = -1;

  printf("Moving to x = %f\n", x_ref);

  while(1){

    //E[i % 3] = - get_angle() * 12;
    //X[i % 3] = 0.2*E[i % 3] - 0.4*E[(i-1) % 3] + 0.2*E[(i-2) % 3] + 1.9*X[(i-1) % 3] - 0.9*X[(i-2) % 3];

    
    x_pos = get_xpos();
    output = (x_ref + 0.03 * get_angle() - x_pos) * K_p;

    if( (x_ref - 0.01) < x_pos && x_pos < (x_ref + 0.01) )
      output = 0;

    printf("X_ref: %3f | Angle: %3f | X_err: %3f | Output: %3f\n", x_ref, 0.03 * get_angle(), x_pos, output);

    i++;
    
    if(output == 0){
      run_motorx(0);
      continue;
    }
    else if(output > 14)
      output = 14;
    else if (output < -14)
      output = -14;
    else if(-5 < output && output < 0)
      output = -5;
    else if (5 > output && output > 0)
      output = 5;


    printf("Readback voltage: %f\n", get_motorx_voltage());

    run_motorx((int)output);
    
    usleep(1000 * 100);
  }  
  
  printf("Jobs done\n");

}
