#include <pthread.h>
#include <cranelib.h>
#include <unistd.h>

#define DATA_PATH "/var/www/html/data/crane/ysteps/"
#define DATA_HEADER "TIME,ANGLE1,ANGLE2,XPOS,YPOS,XTACHO,YTACHO,XVOLT,YVOLT\n"

pthread_t thread_sampler;
FILE * fp;

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

        if ((sample_nr % 10000) == 0 && periods < 4) {
            if (running) {
                printf("GOING UP!!\n");
                printf("MOTOR PWR = %d\n", run_motory(-13));
                running = 0;
            } else {
                printf("GOING DOWN!!\n");
                printf("MOTOR PWR = %d\n", run_motory(13));
                running = 1;
            }
            periods++;
        } else {
            printf("MOTOR PWR = %d\n", run_motory(0));
        }

        usleep(1000);
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