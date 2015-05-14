#ifndef _STACK_H_
#define _STACK_H_

#include "acc.h"


#define STACK_WIDTH 26
#define STACK_HEIGHT 5

#define CONTAINER_WIDTH 0.145
#define CONTAINER_HEIGHT 0.13

#define X_OFF_SET 0.342
#define Y_OFF_SET 1.05


typedef struct stack_loc_s stack_loc_t;
typedef enum {STACK_FREE = 0, STACK_OCCUPIED = 1} loc_status_t;

struct stack_loc_s{
  loc_status_t status;
  point_t loc;
};

void init_stack();
stack_loc_t get_stack_loc(int i, int j);

stack_loc_t update_status(int i, int j, loc_status_t status);
loc_status_t get_status(int i, int j);

#endif
