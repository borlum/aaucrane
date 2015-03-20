#include <stdio.h>
#include <stdlib.h>
#include <comedilib.h>
#include <time.h>

const int range = 0;
const int aref = AREF_GROUND;
const char device_name[] = "/dev/comedi0";
comedi_t *device;

FILE *fp;

pthread_t thread_sampler;

int sensors[] = {0, 1, 10, 2, 3, 4, 9, 10};
int len = sizeof(sensors) / sizeof(int);

void *sampler(void *args) {
  int *channel = args;

  lsampl_t data, maxdata;
  comedi_range *range_info;
  double physical_value;
  int sampl_nr = 0;

  clock_t t_0, t_sample;

  t_0 = clock();

  char tmp[80];
  sprintf(tmp, "/var/www/html/data/crane/xsteps/%d.csv", (int)time(NULL));
  fp = fopen(tmp, "w");
  fprintf(fp, "TIMESTAMP,ANGLE1,ANGLE2,XPOS,YPOS,XTACHO,YTACHO,XVOLT,YVOLT\n");


  while (1) {
    if (sampl_nr == 100) {
      comedi_data_write(device, 1, 0, range, aref, *channel); /* STEP */
    }

    float diff = (((float)t2 - (float)t1) / 1000000.0F ) * 1000; 

    fprintf(fp, "%ld,",  (long)diff);

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
  }
}

int main(int argc, char* argv[]) {
  int channel = 0;
  if(argc == 2){
    channel = atoi(argv[1]);
  }

  printf("Using channel %d\n", channel);

  device = comedi_open(device_name);
  if (device == NULL) {
    printf("Error opening file, %s\n", device_name);
    exit(1);
  }

  /* RESET */
  comedi_data_write(device, 1, 0, range, aref, 2047);
  usleep(5000 * 1000);

  pthread_create(&thread_sampler, NULL, &sampler, &channel);

  while (1) {
    usleep(100 * 1000);
  }

  return 0;
}
