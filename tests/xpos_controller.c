#include <stdlib.h>
#include <unistd.h>
#include <libcrane.h>


static double K_theta = 12;
static double K_p = 26.6;

double notch_filter(double theta);

int main(int argc,char* argv[]){  
  if(argc != 3){
    printf("usage: %s <x y>", argv[0]);
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

  double y_ref = atof(argv[2]);
  double y_pos = 0;
  
  double x_out = -1;
  double y_out = -1;

  printf("Moving to x = %f\n", x_ref);

  while(1){    
    x_pos = get_xpos();
    x_out = (x_ref - x_pos) * K_p;
    
    if( (x_ref - 0.01) < x_pos && x_pos < (x_ref + 0.01) )
      x_out = 0;

    printf("X_ref: %3f | Angle: %3f | X_err: %3f | Output: %3f\n", x_ref, get_angle(), x_pos, x_out);

    i++;
    
    if(x_out == 0){
      run_motorx(0);
/*
      y_pos = get_ypos();
      y_out = (y_ref - y_pos) * 80;

      if( (y_ref - 0.001) < y_pos && y_pos < (y_ref + 0.001) )
	y_out = 0;

      printf("y_ref: %3f | y_pos: %3f | Output: %3f\n", y_ref, y_pos, y_out);

      if(-5 < y_out && y_out < 0)
	y_out = -5;
      else if (5 > y_out && y_out > 0)
	y_out = 5;
*/
      //run_motory((int)y_out);

    }
    else if(x_out > 14)
      x_out = 14;
    else if (x_out < -14)
      x_out = -14;
    else if(-5 < x_out && x_out < 0)
      x_out = -5;
    else if (5 > x_out && x_out > 0)
      x_out = 5;

    run_motorx((int)x_out);

    usleep(1000 * 100);
  }  
  
  printf("Jobs done\n");

}
