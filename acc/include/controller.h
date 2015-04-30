#ifndef _CONTROLLER_H_
#define _CONTROLLER_H

#define X_ERR_BAND 0.015
#define Y_ERR_BAND 0.005
#define C1 10
#define C2 26.3
#define C3 80

#ifdef RTAI
extern RT_TASK *rt_x_axies_controller;
extern RT_TASK *rt_y_axies_controller;
#endif 

void* task_x_axies_controller(void *);
void* task_y_axies_controller(void *);

#endif
