#ifndef _ACC_H_
#define _ACC_H_

typedef enum {DISABLE = 0, ENABLE = 1} magnet_t;
typedef struct crane_cmd_s crane_cmd_t;

struct crane_cmd_s{
  double x_ref[2];
  double y_ref[2];
  double carry_height;
  magnet_t magnet;
};

#endif
