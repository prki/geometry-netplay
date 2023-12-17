#ifndef _TIMER_H_
#define _TIMER_H_
#include <SDL.h>

#define F_TIMER_HISTORY_SIZE 100

typedef struct {
  Uint64 current_time;   // time at initializing F_Timer or after updating
  Uint64 previous_time;  // previous time. 0 upon initialization
  Uint64 delta_time;     // current - previous utility
  double times[F_TIMER_HISTORY_SIZE];  // sampling for FPS
  size_t times_index;                  // internal member for updating times[]
} F_Timer;

void f_timer_init(F_Timer* timer);
void f_timer_update(F_Timer* timer);
double f_timer_calc_fps(const F_Timer* timer);
void f_timer_update_times(F_Timer* timer, double delta);

#endif  // _TIMER_H_
