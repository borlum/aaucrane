#include <pthread.h>
#include <cranelib.h>
#include <unistd.h>

#define DATA_PATH "/var/www/html/data/crane/xsteps/"
#define DATA_HEADER "TIME,ANGLE1,ANGLE2,XPOS,YPOS,XTACHO,YTACHO,XVOLT,YVOLT\n"

pthread_t thread_sampler;
FILE * fp;

void *sampler(void *args)
{
    int step;
    unsigned long t_0, t_sample;
    unsigned int sample_nr = 0;

    t_0 = get_time_micros();
    step = 1;
    while (1) {
        if (step) {
            run_motorx(7);
        }

        /*GRAB TIMESTAMP*/
        t_sample = get_time_micros();
        fprintf(fp, "%ld,",  (t_sample - t_0));

        /*SAMPLE SENSORS*/
        fprintf(fp, "%f,", get_angle_raw());
        fprintf(fp, "\n");
    
        sample_nr++;

        if (sample_nr == 3000) {
            run_motorx(0);
            step = 0;
        }

        usleep(1000);
    }
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("usage: %s [test parameters (e.g. mass + length)]", argv[0]);
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