#include <stdio.h>
#include <stdlib.h>
#include <comedilib.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>

int subdev = 0;
int range = 0;
int aref = AREF_GROUND;
comedi_t *device;

FILE *fp;

const char filename[] = "/dev/comedi0";

int read_and_write(int in_channel, int out_channel) {
	lsampl_t data;
	double physical_value;
	int retval;
	comedi_range * range_info;
	lsampl_t maxdata;

	unsigned int motor_out = 2048;

	retval = comedi_data_read(device, subdev, in_channel, range, aref, &data);
	if(retval < 0)
	{
		comedi_perror(filename);
		return -1;
	}

	motor_out =(((data-2047) * 3000) / 2047) + 500;
	comedi_data_write(device, 1, out_channel, range, aref, motor_out);
}

int dump_sensor_data(){
	lsampl_t data, maxdata;
	comedi_range *range_info;
	double physical_value;
	int i;

	for(i = 0; i < 5; i++){
		comedi_data_read(device, subdev, i, range, aref, &data);
		comedi_set_global_oor_behavior(COMEDI_OOR_NAN);
		range_info = comedi_get_range(device, subdev, i, range);
		maxdata = comedi_get_maxdata(device, subdev, i);
		physical_value= comedi_to_phys(data, range_info, maxdata);
		fprintf(fp, "%g,", physical_value);
		printf("Data from sensor %d: %g\n", i, physical_value);
	}
	fprintf(fp, "\n");
}

int main(int argc, char *argv[])
{
	device = comedi_open(filename);
	lsampl_t data;
	if(device == NULL)
	{
		comedi_perror(filename);
		return -1;
	}

	fp = fopen("crane.data", "w");

	if(fp == NULL)
		printf("FUCK NO. could not open file\n");

	fprintf(fp, "ANGLE,XPOS,YPOS,XTACHO,YTACHO\n");

	while(1)
	{

		read_and_write(14, 0);
		read_and_write(15, 1);
		dump_sensor_data();
		usleep(250 * 1000); /*ms -> us*/
	}
	return -1;
}
