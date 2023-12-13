#include "f_timer.h"

#include <SDL.h>
#include <string.h>

void f_timer_init(F_Timer* timer) {
  timer->current_time = SDL_GetTicks64();
  timer->previous_time = 0;
  timer->delta_time = timer->current_time - timer->previous_time;
  timer->times_index = 0;

  memset(timer->times, 0, sizeof(Uint64) * F_TIMER_HISTORY_SIZE);
}

// Internal/private function for working with the history time sample
void f_timer_update_times(F_Timer* timer) {
  if (timer->times_index >= F_TIMER_HISTORY_SIZE) {
    timer->times_index = 0;
  }

  timer->times[timer->times_index] = timer->delta_time;
  timer->times_index += 1;
}

void f_timer_update(F_Timer* timer) {
  timer->previous_time = timer->current_time;
  timer->current_time = SDL_GetTicks64();
  timer->delta_time = timer->current_time - timer->previous_time;

  f_timer_update_times(timer);
}

double f_timer_calc_fps(const F_Timer* timer) {
  Uint64 deltas = 0;
  for (size_t i = 0; i < F_TIMER_HISTORY_SIZE; i++) {
    deltas += timer->times[i];
  }
  double avg_delta = (double)deltas / (double)F_TIMER_HISTORY_SIZE;

  double ret = (1000.0 / avg_delta);
  return ret;
}
