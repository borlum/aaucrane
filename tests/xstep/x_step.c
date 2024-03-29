#include <pthread.h>
#include <libcrane.h>
#include <unistd.h>

#define DATA_PATH "/var/www/html/data/crane/xsteps/"
#define DATA_HEADER "TIME,ANGLE1,ANGLE2,XPOS,YPOS,XTACHO,YTACHO,XVOLT,YVOLT\n"

pthread_t thread_sampler;
FILE * fp;

#ifdef RTAI
#include <rtai_lxrt.h>
RT_TASK *rt_sampler;
#endif

const static float step_size = 6.5;

void *sampler(void *args)
{
    int step;
    unsigned long t_0, t_sample;
    unsigned int sample_nr = 0;

    t_0 = get_time_micros();
    step = 1;

#ifdef RTAI
    /*Transform to RTAI task*/
    RTIME period = nano2count(1E6);
    if(!(rt_sampler = rt_task_init_schmod(nam2num("rt_sampler"), 1, 0, 0, SCHED_FIFO, 0))){
      printf("rt_sampler\n");
      exit(42);
    }
    rt_task_make_periodic(rt_sampler, rt_get_time() + period, period);
    rt_make_hard_real_time();
    printf("Started rt_sampler\n");
#endif

    
    while (1) {
        if (step) {
	  printf("Comedi out %d\n", run_motory(step_size));
	    step = 0;	    
        }

        /*GRAB TIMESTAMP*/
        t_sample = get_time_micros();
        fprintf(fp, "%ld,",  (t_sample - t_0));

        /*SAMPLE SENSORS*/
        fprintf(fp, "%f,", -1.0);
        fprintf(fp, "%f,", get_angle());
        fprintf(fp, "%f,", get_xpos());
        fprintf(fp, "%f,", get_ypos());
        fprintf(fp, "%f,", get_motorx_velocity());
        fprintf(fp, "%f,", get_motory_velocity());
        fprintf(fp, "%f,", get_motorx_voltage());
        fprintf(fp, "%f,", get_motory_voltage());
        fprintf(fp, "\n");
    
        sample_nr++;

        if (sample_nr == 3000) {
            run_motory(0);
        }
	
#ifdef RTAI
	rt_task_wait_period();
#else
	usleep(1000);
#endif
    }
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("usage: %s \"[test parameters (e.g. mass + length)]\"\n", argv[0]);
        return 0;
    }

    initialize_crane();

    /* RESET */
    run_motorx(0);
    run_motory(0);

    /*PREPARE FILE*/
    char tmp[160];
    sprintf(tmp, "%s/%d.csv", DATA_PATH, (int)time(NULL));
    fp = fopen(tmp, "w");
    fprintf(fp, "%s\n", argv[1]);
    fprintf(fp, DATA_HEADER);

    pthread_create(&thread_sampler, NULL, &sampler, NULL);

    while (1) {
        usleep(100 * 1000);
    }

    return 0;
}
