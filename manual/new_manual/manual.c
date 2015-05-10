#include <libcrane.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

#include "compensator.h"

#define DATA_PATH "/var/www/html/data/crane/manual/"
#define DATA_HEADER "TIME,ANGLE1,ANGLE2,XPOS,YPOS,XTACHO,YTACHO,XVOLT,YVOLT\n"

FILE * fp;

int main(int argc, char* argv[])
{
  double manual_x, manual_y;
  double angle_out, vel, out;
  initialize_crane();

  /*PREPARE FILE*/
  char tmp[160];
  sprintf(tmp, "%s/%d.csv", DATA_PATH, (int)time(NULL));
  fp = fopen(tmp, "w");
  fprintf(fp, DATA_HEADER);

  unsigned long t_0, t_sample;

  t_0 = get_time_micros();
  while(1){
    printf("JOYPAD: (x,y) = (%3f, %3f)\n", get_ctrlpad_x(), get_ctrlpad_y());
    printf("X_pos: %3f @ %3f| Y_pos: %3f @ %3f | ANGLE: %3f\n", get_xpos(), get_xpos_raw(), get_ypos(), get_ypos_raw(), get_angle() * (180.0/3.14));

    if( fabs((manual_x = get_ctrlpad_x())) < 2 )
      manual_x = 0;      
    
    if( fabs((manual_y = get_ctrlpad_y())) < 2 )
      manual_y = 0;

    
    angle_out = angle_controller(-get_angle());
    vel = get_x_velocity();
    out = velocity_controller_x(angle_out - vel);

    if(get_ctrlpad_ctrl_switch()){
      out += get_ctrlpad_x() * .5;
    } else {
      out = get_ctrlpad_x();
    }
    
    run_motorx(out);
    run_motory(manual_y);

    if (get_ctrlpad_magnet_switch()) {
        enable_magnet();
    } else {
        disable_magnet();
    }

    /*GRAB TIMESTAMP*/
    t_sample = get_time_micros();
    fprintf(fp, "%ld,",  (t_sample - t_0));

    /*SAMPLE SENSORS*/
    fprintf(fp, "%f,", get_old_angle_raw());
    fprintf(fp, "%f,", get_angle_raw());
    fprintf(fp, "%f,", get_xpos_raw());
    fprintf(fp, "%f,", get_ypos_raw());
    fprintf(fp, "%f,", get_motorx_velocity_raw());
    fprintf(fp, "%f,", get_motory_velocity_raw());
    fprintf(fp, "%f,", get_motorx_voltage());
    fprintf(fp, "%f,", get_motory_voltage());
    fprintf(fp, "\n");


    usleep(1000 * 1);
  }
}
