#include <pthread.h>
#include <cranelib.h>
#include <unistd.h>

pthread_t thread_sampler;

void *sampler(void *args)
{
    unsigned long t_0, t_sample;

    char tmp[80];
    sprintf(tmp, "/var/www/html/data/crane/xsteps/%d.csv", (int)time(NULL));
    fp = fopen(tmp, "w");
    fprintf(fp, "TIMESTAMP,ANGLE1,ANGLE2,XPOS,YPOS,XTACHO,YTACHO,XVOLT,YVOLT\n");
  
    int step = 1;

    while (1) {
        if (step) {
            /*GO*/
        }

        fprintf(fp, "%ld,",  (t_sample - t_0));
        fprintf(fp, "\n");
    
        sampl_nr++;

        if (sampl_nr == 3000) {
            step = 0;
        }
        usleep(1000);
    }
}

int main(int argc, char* argv[])
{
    /* RESET */

    pthread_create(&thread_sampler, NULL, &sampler, NULL);

    while (1) {
        usleep(100 * 1000);
    }

    return 0;
}