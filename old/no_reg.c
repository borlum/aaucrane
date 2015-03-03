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

const char filename[] = "/dev/comedi0";

int read_and_write(int in_channel, int out_channel) {
	lsampl_t data;
	double physical_value;
	int retval;
	comedi_range * range_info;
	lsampl_t maxdata;

	unsigned int motorX_out = 2048;

	retval = comedi_data_read(device, subdev, in_channel, range, aref, &data);
	if(retval < 0)
	{
		comedi_perror(filename);
		return -1;
	}

	comedi_set_global_oor_behavior(COMEDI_OOR_NAN);
	range_info = comedi_get_range(device, subdev, in_channel, range);
	maxdata = comedi_get_maxdata(device, subdev, in_channel);
	printf("[0,%d] -> [%g,%g]\n", maxdata, range_info->min, range_info->max);
	physical_value = comedi_to_phys(data, range_info, maxdata);
	if(isnan(physical_value)) {
		printf("Out of range [%g,%g]", range_info->min, range_info->max);
	} else {
		printf("%g", physical_value);
		switch(range_info->unit)
		{
			case UNIT_volt: printf(" V"); break;
			case UNIT_mA: printf(" mA"); break;
			case UNIT_none: break;
			default: printf(" (unknown unit %d)", range_info->unit);
		}
	}
	//motorX_out = (unsigned int) (((float) (data - 1024)) * 0.6);
	motorX_out =(((data-2047) * 3000) / 2047) + 500;
	comedi_data_write(device, 1, out_channel, range, aref, motorX_out);
	printf(" (%lu in raw units)\n", (unsigned long)data);
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

	while(1)
	{

		read_and_write(14, 0);
		read_and_write(15, 1);
		//comedi_data_read(device, subdev, 15, range, aref, &data);
		//printf("Data: %lu\n", data);
		usleep(250 * 1000); /*ms -> us*/
	}
	return -1;
}
