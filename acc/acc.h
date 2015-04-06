#ifndef _ACC_H_
#define _ACC_H_

/* Qeueue related stuff */
#define BUFFER_SIZE 2 * sizeof(crane_cmd_t)

#define Q_TO_X "/ACC_to_x"
#define Q_FROM_X "/ACC_from_x"

#define Q_TO_Y "/ACC_to_y"
#define Q_FROM_Y "/ACC_from_y"

#define Q_TO_C "/ACC_to_c"
#define Q_FROM_C "/ACC_from_c"


typedef enum {DISABLE = 0, ENABLE = 1} magnet_t;
typedef struct crane_cmd_s crane_cmd_t;

struct crane_cmd_s{
  double x_ref[2];
  double y_ref[2];
  double carry_height;
  magnet_t magnet;
};

#endif
