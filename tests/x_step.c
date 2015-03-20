#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <comedilib.h>
#include <pthread.h>

#define DIGITAL_IO_SUBDEV 2
#define MAGNET_ENABLE 7

const int range = 0;
const int aref = AREF_GROUND;
const char device_name[] = "/dev/comedi0";
comedi_t *device;

FILE *fp;

pthread_t thread_sampler;

int sensors[] = {0, 1, 10, 2, 3, 4, 9, 10};
int len = sizeof(sensors) / sizeof(int);

int payload_length;
int payload_weight;

void *sampler(void *args) {
  lsampl_t data, maxdata;
  comedi_range *range_info;
  double physical_value;
  int sampl_nr = 0;

  const unsigned long long nano = 1000000000;
  unsigned long t_0, t_sample;
  struct timespec tm;

  clock_gettime(CLOCK_REALTIME, &tm);
  t_0 = (tm.tv_nsec + tm.tv_sec * nano) / 1000;

  char tmp[80];
  sprintf(tmp, "/var/www/html/data/crane/xsteps/%d.csv", (int)time(NULL));
  fp = fopen(tmp, "w");
  fprintf(fp, "WEIGHT: %d, LENGTH: %d\n", payload_weight, payload_length);
  fprintf(fp, "TIMESTAMP,ANGLE1,ANGLE2,XPOS,YPOS,XTACHO,YTACHO,XVOLT,YVOLT\n");

  while (1) {
    if (sampl_nr == 100) {
      comedi_data_write(device, 1, 0, range, aref, 4094); /* STEP */
    }

    clock_gettime(CLOCK_REALTIME, &tm);
    t_sample = (tm.tv_nsec + tm.tv_sec * nano) / 1000; 

    fprintf(fp, "%ld,",  (t_sample - t_0));

    for (int i = 0; i < len; i++) {
      /* Log data */
      comedi_data_read(device, 0, sensors[i], range, aref, &data);
      comedi_set_global_oor_behavior(COMEDI_OOR_NAN);
      range_info     = comedi_get_range(device,   0, sensors[i], range);
      maxdata        = comedi_get_maxdata(device, 0, sensors[i]);
      physical_value = comedi_to_phys(data, range_info, maxdata);
      fprintf(fp, "%g,", physical_value);
    }
    fprintf(fp, "\n");
    sampl_nr++;
    usleep(1000);
  }
}

int main(int argc, char* argv[]) {
  device = comedi_open(device_name);
  if (device == NULL) {
    printf("Error opening file, %s\n", device_name);
    exit(1);
  }

  printf("TEST PARAMETERS => weight, length\n");
  scanf("%i, %i", &payload_weight, &payload_length);
  printf("THANK YOU! => RUNNING TEST!\n");
  

  comedi_dio_config(device, DIGITAL_IO_SUBDEV, MAGNET_ENABLE, COMEDI_OUTPUT);
  comedi_dio_write(device, DIGITAL_IO_SUBDEV, MAGNET_ENABLE, 1);
  /* RESET */
  comedi_data_write(device, 1, 0, range, aref, 2047);
  usleep(5000 * 1000);

  pthread_create(&thread_sampler, NULL, &sampler, NULL);

  while (1) {
    usleep(100 * 1000);
  }

  return 0;
}
