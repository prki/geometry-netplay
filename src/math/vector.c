#include "vector.h"

#include <math.h>
#include <stdio.h>

#include "trigonometry.h"

void vec2d_print(const vec2d* vec) {
  printf("[DBG] Vec2d: x: %.2f y: %.2f\n", vec->x, vec->y);
}

double vec2d_magnitude(const vec2d* vec) {
  // return sqrt(pow(vec->x, 2) + pow(vec->y, 2));
  return sqrt(vec->x * vec->x + vec->y * vec->y);
}

// Rotates a vector, assuming the vector is purely
// directional, i.e. is defined via origin
// Angle defined in degrees
// [TODO] Is a rotated unit vector still a unit vector?
vec2d vec2d_rotate(const vec2d* vec, double angle) {
  double x2, y2;
  angle = deg_to_rad(angle);
  x2 = vec->x * cos(angle) - vec->y * sin(angle);
  y2 = vec->x * sin(angle) + vec->y * cos(angle);

  vec2d ret = {.x = x2, .y = y2};

  return ret;
}

// Returns a new normalized vector
vec2d vec2d_normalize(const vec2d* vec) {
  double mag = vec2d_magnitude(vec);

  double x = vec->x / mag;
  double y = vec->y / mag;

  vec2d normalized_vec = {.x = x, .y = y};

  return normalized_vec;
}

vec2d vec2d_scalar_multiply(const vec2d* vec, const double scalar) {
  vec2d mult_vec = {.x = vec->x * scalar, .y = vec->y * scalar};

  return mult_vec;
}

// Get vector between points A and B (defined as (A1,A2) and (B1,B2)
vec2d vec2d_from_points(double a1, double a2, double b1, double b2) {
  double x = b1 - a1;
  double y = b2 - a2;

  vec2d ret = {.x = x, .y = y};

  return ret;
}

double vec2d_dot_product(const vec2d* a, const vec2d* b) {
  return (a->x * b->x) + (a->y * b->y);
}

/* (a . b) / (b . b) * b where . signifies dot product, * scalar product*/
vec2d vec2d_project(const vec2d* vec, const vec2d* tgt) {
  vec2d ret = {.x = NAN, .y = NAN};
  if (fabs(tgt->x) < 1e-10 && fabs(tgt->y) < 1e-10) {
    printf(
        "[WARN] Attempted vec projection on a (0, 0) vector; Returned NAN "
        "NAN\n");
    return ret;
  }
  double top = vec2d_dot_product(vec, tgt);
  double bottom = vec2d_dot_product(tgt, tgt);

  double div = top / bottom;
  ret = vec2d_scalar_multiply(tgt, div);

  return ret;
}

// Utility function calculating the center point of a vector
// between two points. => only worked for direct lines I guess?
vec2d vec2d_midpoint(const vec2d* vec1, const vec2d* vec2) {
  vec2d center = {.x = (vec2->x - vec1->x) / 2.0,
                  .y = (vec2->y - vec1->y) / 2.0};
  vec2d ret = {.x = vec1->x + center.x, .y = vec1->y + center.y};

  return ret;
}

// Utility function returning a vector's normal. The normal is not normalized,
// i.e. does not have magnitude == 1
vec2d vec2d_normal(const vec2d* vec) {
  vec2d ret = {.x = -1 * vec->y, .y = vec->x};

  return ret;
}

double vec2d_angle(const vec2d* vec1, const vec2d* vec2) {
  double top = vec2d_dot_product(vec1, vec2);
  double mag1 = vec2d_magnitude(vec1);
  double mag2 = vec2d_magnitude(vec2);
  double bottom = mag1 * mag2;
  if (bottom == 0) {
    printf("[WARN] Attempted to calculate angle between 0 vec\n");
    return NAN;
  }

  double angle = top / bottom;

  return rad_to_deg(acos(angle));
}

vec2d rotate_point(vec2d point, vec2d pivot, double angle) {
  angle = deg_to_rad(angle);

  // translated with respect to origin (0, 0)
  double tempX = point.x - pivot.x;
  double tempY = point.y - pivot.y;

  double x = tempX * cos(angle) - tempY * sin(angle);
  double y = tempX * sin(angle) + tempY * cos(angle);

  // translate back
  vec2d ret = {.x = x + pivot.x, .y = y + pivot.y};

  return ret;
}

// Utility function calculating determinant of a square matrix dim=2
// was this just a1 * b2 - a2 * b1?
double det_2d(double A[2][2]) { return A[0][0] * A[1][1] - A[0][1] * A[1][0]; }

// Function finding an intersection point between lines P and Q, defined via
// a direction vector and a point.
// This is solved by creating a system of linear equations in a matrix
// format A * X = B, where A is a matrix of vector values (parameterized line
// equation)/coefficient values and B represents the constant vector.
// X = [s t] (assuming common parameter names); X = A^-1 * B
// A^-1 is calculated statically as det_A_inv * A
vec2d line_intersection(const vec2d* vec1, const vec2d* point1,
                        const vec2d* vec2, const vec2d* point2) {
  double A[2][2] = {{vec1->x, -vec2->x}, {vec1->y, -vec2->y}};
  const double B[2] = {point2->x - point1->x, point2->y - point1->y};
  double det_A = det_2d(A);
  if (fabs(det_A) < 10e-7) {
    return (vec2d){.x = NAN, .y = NAN};
  }
  double det_A_inv = 1.0 / det_A;

  double A_inv[2][2] = {{A[1][1] * det_A_inv, A[0][1] * -1 * det_A_inv},
                        {A[1][0] * -1 * det_A_inv, A[0][0] * det_A_inv}};

  double t = A_inv[0][0] * B[0] + A_inv[0][1] * B[1];
  // double s = A_inv[1][0] * B[0] + A_inv[1][1] * B[1];  // not necessary

  double intersection_x = point1->x + t * vec1->x;
  double intersection_y = point1->y + t * vec1->y;

  return (vec2d){.x = intersection_x, .y = intersection_y};
}
