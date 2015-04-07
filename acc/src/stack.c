#include "stack.h"

stack_loc_t stack[STACK_WIDTH][STACK_HEIGHT];

void init_stack(){
  for(int i = 0; i < STACK_WIDTH; i++){
    for(int j = 0; j < STACK_HEIGHT; j++){
      double x = (i * CONTAINER_WIDTH) + X_OFF_SET;
      double y = Y_OFF_SET - (j * CONTAINER_HEIGHT);
      stack[i][j].loc.x = x;
      stack[i][j].loc.y = y;
      stack[i][j].status = FREE;
    }
  }
}

stack_loc_t get_stack_loc(int i, int j){
  return stack[i][j];
}

stack_loc_t update_status(int i, int j, loc_status_t status){
  stack[i][j].status = status;
  return stack[i][j];
}


loc_status_t get_status(int i, int j){
  return stack[i][j].status;
}
