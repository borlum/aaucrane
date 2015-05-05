#ifndef _CONTROLLER_H_
#define _CONTROLLER_H

#define X_ERR_BAND 0.01
#define Y_ERR_BAND 0.005
#define ANGLE_ERR_BAND 0.03
#define SETTLE_HITS 250

#ifdef RTAI
extern RT_TASK *rt_x_axis_controller;
extern RT_TASK *rt_y_axis_controller;
#endif

void* task_x_axis_controller(void *);
void* task_y_axis_controller(void *);

#endif