#ifndef _F_CONFIG_H_
#define _F_CONFIG_H_

// [TODO] This could include more things, such as resolution, windowed, ...
typedef struct {
  int r_vsync;
  unsigned int r_max_fps;
} F_Config;

#endif  // _F_CONFIG_H_
