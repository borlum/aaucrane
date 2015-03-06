#include <stdio.h>
#include <stdlib.h>
#include <comedilib.h>
#include <rtai_lxrt.h>
#include <pthread.h>

#define TICK_TIME 1E3

const int range = 0;
const int aref = AREF_GROUND;
const char device_name[] = "/dev/comedi0";
comedi_t *device;

RT_TASK *rt_sampler;
pthread_t *thread_sampler;

void *sampler(void *args) {
    RTIME t_sample = nano2count(10 * TICK_TIME);
    RTIME t_expected = rt_get_time() + t_sample;

    int ID = nam2num("sampler");
    if (!(rt_sampler = rt_task_init_schmod(ID,1,0,0,SCHED_FIFO,0))) {
        printf("ERROR: Could not init. task [TASK2]\n");
        exit(1); 
    }

    rt_task_make_periodic(rt_sampler, t_expected, t_sample);
    rt_make_hard_real_time();

    RTIME t_pre = rt_get_time();
    while (1) {
        printf("T = %d;\n", (int)rt_get_time() - t_pre);
        t_pre = rt_get_time();
        rt_task_wait_period();
    }

    rt_task_delete(rt_sampler);
}

int main() {
    device = comedi_open(device_name);
    if (device == NULL) {
        printf("Error opening file, %s\n", device_name);
        exit(1);
    }

    pthread_create(thread_sampler, NULL, &sampler, NULL);

    while (1) {
        printf("See ya!\n");
        usleep(100 * 1000);
    }

    return 0;
}