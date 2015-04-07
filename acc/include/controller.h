#ifndef _CONTROLLER_H_
#define _CONTROLLER_H

#define X_ERR_BAND 0.005
#define Y_ERR_BAND 0.005
#define C1 10
#define C2 26.3
#define C3 80

void* rt_x_axies_controller(void *);
void* rt_y_axies_controller(void *);

#endif
