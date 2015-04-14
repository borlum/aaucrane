#include <libcrane.h>
#include <unistd.h>
#include <stdlib.h>

#define DATA_PATH "/var/www/html/data/crane/manual/"
#define DATA_HEADER "TIME,ANGLE1,ANGLE2,XPOS,YPOS,XTACHO,YTACHO,XVOLT,YVOLT\n"

int main(int argc, char* argv[])
{
  initialize_crane();

  /*PREPARE FILE*/
  char tmp[160];
  sprintf(tmp, "%s/%d.csv", DATA_PATH, (int)time(NULL));
  fp = fopen(tmp, "w");
  fprintf(fp, DATA_HEADER);

  unsigned long t_0, t_sample;

  t_0 = get_time_micros();
  while(1){
#ifdef ANGLE
    printf("Raw measurement: %f | Converted to rad: %f | Converted to deg: %f\n", get_angle_raw(), get_angle(), get_angle() * (180.0/3.14));
#else
    printf("JOYPAD: (x,y) = (%3f, %3f)\n", get_ctrlpad_x(), get_ctrlpad_y());
    printf("X_pos: %3f @ %3f| Y_pos: %3f @ %3f | ANGLE: %3f\n", get_xpos(), get_xpos_raw(), get_ypos(), get_ypos_raw(), get_angle() * (180.0/3.14));
    run_motorx(get_ctrlpad_x());
    run_motory(get_ctrlpad_y());

    if (get_ctrlpad_magnet_switch()) {
        enable_magnet();
    } else {
        disable_magnet();
    }
#endif

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


    usleep(1000 * 100);
  }
}
