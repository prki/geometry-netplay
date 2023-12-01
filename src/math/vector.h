#ifndef _VECTOR_H_
#define _VECTOR_H_

typedef struct vec2d {
  double x;
  double y;
} vec2d;

void vec2d_print(const vec2d* vec);
double vec2d_magnitude(const vec2d* vec);
vec2d vec2d_normalize(const vec2d* vec);
vec2d vec2d_scalar_multiply(const vec2d* vec, const double scalar);
vec2d vec2d_project(const vec2d* vec, const vec2d* tgt);
double vec2d_dot_product(const vec2d* a, const vec2d* b);
vec2d vec2d_rotate(const vec2d* vec, double angle);
vec2d vec2d_normal(const vec2d* vec);

vec2d vec2d_from_points(double a1, double a2, double b1, double b2);
vec2d vec2d_midpoint(const vec2d* vec1, const vec2d* vec2);
double vec2d_angle(const vec2d* vec1, const vec2d* vec2);
vec2d rotate_point(vec2d point, vec2d pivot, double angle);

// Analytical detinifition of a line
typedef struct Line {
  vec2d direction;
  vec2d point;
} Line;

// [TODO] Rewrite to use lines instead of 4 params
vec2d line_intersection(const vec2d* vec1, const vec2d* point1,
                        const vec2d* vec2, const vec2d* point2);

#endif  // _VECTOR_H_
