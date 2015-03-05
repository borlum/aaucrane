#include <rtai_lxrt.h>
#include <stdio.h>
#include <pthread.h>

#define TICK_TIME 3E9

int hard_rt_running;
int hard_rt_continue;

/*RT Tasks*/
static RT_TASK *t1;
static RT_TASK *t2;

/*Threads*/
static pthread_t foo_thread;

void task_body(void *args) {
    /*Sampling time*/
    RTIME t_sample = (RTIME *) args;
    /*Expected start time of task*/
    RTIME expected;

    printf("WOOOOOW!\n");

    if (!(t2 = rt_task_init_schmod(nam2num("TASK2"),2,0,0,SCHED_FIFO,0))) {
        printf("ERROR: Could not init. task [TASK1]\n");
        exit(1); 
    }

    expected = rt_get_time() + t_sample;
    rt_task_make_periodic(t2, expected, t_sample);
    rt_make_hard_real_time();

    int i = 0;
    float pre_time = 0;
    while (1) {
        i++;
        printf("[TASK2] => %f s\n", (float)rt_get_time_ns()/1E9 - pre_time);
        pre_time = (float)rt_get_time_ns()/1E9;
        if (i == 5) {
            hard_rt_continue = 0;
            break;
        }
        /*Sleep...*/
        rt_task_wait_period();
    }

    rt_task_delete(t2);
}

int main() {
    hard_rt_running = 1;
    hard_rt_continue = 1;
    static RTIME t_sample;

    /*Initialize timer*/
    if (hard_rt_running == rt_is_hard_timer_running()) {
        printf("Real-Time Timer Running...\n");
    } else {
        printf("Starting Real-Time Timer...\n");
        rt_set_oneshot_mode();
        start_rt_timer(0);
    }
    t_sample = nano2count(TICK_TIME);

    /*Name, prio., stack size, msg. size, policy, CPUs allowed*/
    if (!(t1 = rt_task_init_schmod(nam2num("TASK1"),1,0,0,SCHED_FIFO,0))) {
        printf("ERROR: Could not init. task [TASK1]\n");
        exit(1);
    }

    pthread_create(&foo_thread, NULL, task_body, (void *)t_sample);
    printf("WOOOOOW...!\n");

    while (hard_rt_continue);

    /*TERMINATION*/
    rt_task_delete(t1);
    printf("DONE!\n");
    return 0;
}