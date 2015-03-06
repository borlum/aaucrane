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

int run = NULL;

RT_TASK *rt_sampler;
pthread_t thread_sampler;

void sig_handler(int sig) {
    run = NULL;
    usleep(100 * 1000);
    fclose(fp); 
}

void *sampler(void *args) {
    RTIME t_sample = nano2count(10 * TICK_TIME);
    RTIME t_expected = rt_get_time() + t_sample;

    int ID = nam2num("sampler");
    if (!(rt_sampler = rt_task_init_schmod(ID,1,0,0,SCHED_FIFO,0))) {
        printf("ERROR: Could not init. task [sampler]\n");
        exit(1); 
    }

    rt_task_make_periodic(rt_sampler, t_expected, t_sample);
    rt_make_hard_real_time();
    fp = fopen("data.csv", "w");
    while (run) {
        fprintf(fp, "T = %d ns;\n", (unsigned int)rt_get_time_ns());
        t_pre = (unsigned int)rt_get_time_ns();
        rt_task_wait_period();
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

    run = 1;

    pthread_create(&thread_sampler, NULL, &sampler, NULL);

    while (run) {
        usleep(100 * 1000);
    }

    return 0;
}