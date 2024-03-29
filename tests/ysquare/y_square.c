#include <pthread.h>
#include <libcrane.h>
#include <unistd.h>
#include <stdlib.h>

#define DATA_PATH "/var/www/html/data/crane/ysquare/"
#define DATA_HEADER "TIME,ANGLE1,ANGLE2,XPOS,YPOS,XTACHO,YTACHO,XVOLT,YVOLT\n"

pthread_t thread_sampler;
FILE * fp;

int t_on          = 0;
int nr_of_periods = 0;

void *sampler(void *args)
{
    int periods, running;
    unsigned long t_0, t_sample;
    unsigned int sample_nr = 0;

    t_0 = get_time_micros();

    periods = 0;
    running = 0;
    while (1) {
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
    
        sample_nr++;

        if ((sample_nr % t_on) == 0 && periods < nr_of_periods) {
            if (running) {
                run_motory(-14);
                running = 0;
            } else {
                run_motory(14);
                running = 1;
            }
            periods++;
        } else if (periods == nr_of_periods) {
            run_motory(0);
        }

        usleep(1000);
    }
}

int main(int argc, char* argv[])
{
    if (argc != 4) {
        printf("usage: %s N T \"[test parameters (e.g. mass + length)]\"\n", argv[0]);
        return 0;
    }

    nr_of_periods = strtol(argv[1], NULL, 10);
    t_on          = strtol(argv[2], NULL, 10);

    initialize_crane();

    /* RESET */
    run_motorx(0);
    run_motory(0);

    /*PREPARE FILE*/
    char tmp[160];
    sprintf(tmp, "%s/%d.csv", DATA_PATH, (int)time(NULL));
    fp = fopen(tmp, "w");
    fprintf(fp, "%s\n", argv[3]);
    fprintf(fp, DATA_HEADER);

    pthread_create(&thread_sampler, NULL, &sampler, NULL);

    while (1) {
        usleep(100 * 1000);
    }

    return 0;
}
