#include "trigonometry.h"

#include <math.h>

double deg_to_rad(double deg) { return deg * (M_PI / 180.0); }

double rad_to_deg(double rad) { return rad * (180.0 / M_PI); }

// Utility function converting degrees in a unit circle to
// an actual vector. Degree 0 represents (0, 1) as the gameworld
// has rotated axes.
// Returns a normalized vector
vec2d deg_to_vec2d(double deg) {
  deg -= 90;  // rotate by 90 so that cos(0) == 1, sin(0) == 0
  double x = cos(deg_to_rad(deg));
  double y = sin(deg_to_rad(deg));

  vec2d ret = {.x = x, .y = y};

  return ret;
}
