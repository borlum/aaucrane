#ifndef _CONTROLLER_H_
#define _CONTROLLER_H

#define X_ERR_BAND 0.0075
#define Y_ERR_BAND 0.005
#define ANGLE_ERR_BAND 0.03
#define SETTLE_HITS 25

void* task_x_axis_controller(void *);
void* task_y_axis_controller(void *);
void* task_logger(void *);

int init_logger(const char *data_path, size_t len);

int enable_logger();
int disable_logger();

int get_enable_logger();
int get_new_log();

#endif
