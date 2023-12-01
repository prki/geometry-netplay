#ifndef _SHAPES_H_
#define _SHAPES_H_
#include "vector.h"

/*
 * Assuming rotation angle 0:
 *  A1 --- A2
 *  |      |
 *  |      |
 *  A3 --- A4
 */
typedef struct Rectangle {
  vec2d a1;
  vec2d a2;
  vec2d a3;
  vec2d a4;
  vec2d origin_topleft;
  double width;
  double height;
  double rot_angle;  // [TODO] Player direction vector - once shooting matters,
                     // would that be of benefit? sdl uses angle - might want
                     // this to be vec
} Rectangle;

// [TODO] It might be cool to also initialize new with rotation angle
Rectangle new_rectangle(double posX, double posY, double width, double height);
void rotate_rectangle(Rectangle* rectangle, double angle);
void translate_rectangle(Rectangle* rectangle, vec2d trnsl_magnitude);
int rectangle_intersection(const Rectangle* r1, const Rectangle* r2);
vec2d rectangle_center(const Rectangle* rect);
void find_rect_line_intersection(vec2d intersection[2], const Rectangle* rect,
                                 vec2d point, vec2d direction);
int point_in_rectangle(const vec2d* point, const Rectangle* rect);
void rectangle_axes(vec2d axes[2], const Rectangle* rect);

void print_rectangle(const Rectangle* rect);

typedef struct Circle {
  vec2d position;
  double radius;
} Circle;

Circle new_circle(double posX, double posY, double radius);
void translate_circle(Circle* circle, vec2d trnsl_magnitude);
int circle_rectangle_intersection(const Circle* circ, const Rectangle* rect);

#endif  // _SHAPES_H_
