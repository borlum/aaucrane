#include <stdio.h>
#include <stdlib.h>
#include <comedilib.h>
#include <rtai_lxrt.h>
#include <pthread.h>

#define TICK_TIME 1E6

const int range = 0;
const int aref = AREF_GROUND;
const char device_name[] = "/dev/comedi0";
comedi_t *device;

FILE *fp;

int run = 0;

RT_TASK *rt_sampler;
pthread_t thread_sampler;

int sensors[] = {0, 1, 2, 3, 4, 9, 10};
int len = sizeof(sensors) / sizeof(int);

void sig_handler(int sig) {
    run = 0;
    usleep(100 * 1000);
    fclose(fp); 
}

void *sampler(void *args) {
    RTIME t_sample = nano2count(10 * TICK_TIME);
    RTIME t_expected = rt_get_time() + t_sample;

    lsampl_t data, maxdata;
    comedi_range *range_info;
    double physical_value;
    int sampl_nr = 0;

    int ID = nam2num("sampler");
    if (!(rt_sampler = rt_task_init_schmod(ID,1,0,0,SCHED_FIFO,0))) {
        printf("ERROR: Could not init. task [sampler]\n");
        exit(1); 
    }

    rt_task_make_periodic(rt_sampler, t_expected, t_sample);
    rt_make_hard_real_time();

    fp = fopen("data.csv", "w");
    fprintf(fp, "TIMESTAMP,ANGLE,XPOS,YPOS,XTACHO,YTACHO,XVOLT,YVOLT\n");
    RTIME t_init = rt_get_time_ns();
    while (run) {
        if (sampl_nr == 100) {
            comedi_data_write(device, 1, 0, range, aref, 0);
        }

        fprintf(fp, "%lld,", rt_get_time_ns() - t_init);

        for (int i = 0; i < len; i++) {
            comedi_data_read(device, 0, sensors[i], range, aref, &data);
            comedi_set_global_oor_behavior(COMEDI_OOR_NAN);
            range_info     = comedi_get_range(device,   0, sensors[i], range);
            maxdata        = comedi_get_maxdata(device, 0, sensors[i]);
            physical_value = comedi_to_phys(data, range_info, maxdata);
            fprintf(fp, "%g,", physical_value);
        }
        fprintf(fp, "\n");
        rt_task_wait_period();

        sampl_nr++;
    }

    rt_task_delete(rt_sampler);
}

int main() {
    signal(SIGINT, sig_handler);
    device = comedi_open(device_name);
    if (device == NULL) {
        printf("Error opening file, %s\n", device_name);
        exit(1);
    }

    /* RESET */
    comedi_data_write(device, 1, 0, range, aref, 0);
    usleep(5000 * 1000);

    run = 1;

    pthread_create(&thread_sampler, NULL, &sampler, NULL);

    while (run) {
        usleep(100 * 1000);
    }

    return 0;
}