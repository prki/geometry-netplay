#include "shapes.h"

#include <stdio.h>

#include "float.h"
#include "math.h"
#include "trigonometry.h"
#include "vector.h"

// Function initializing a new rectangle. Width and height are expected to be
// positive values. While the function won't throw an error and likely the
// rectangle will be valid with nonpositive width and height, calculations
// are likely not to work correctly. Function does not perform any checks.
Rectangle new_rectangle(double posX, double posY, double width, double height) {
  Rectangle ret;

  ret.origin_topleft.x = posX;
  ret.origin_topleft.y = posY;

  ret.a1.x = posX;
  ret.a1.y = posY;

  ret.a2.x = posX + width;
  ret.a2.y = posY;

  ret.a3.x = posX;
  ret.a3.y = posY + height;

  ret.a4.x = posX + width;
  ret.a4.y = posY + height;

  ret.width = width;
  ret.height = height;
  ret.rot_angle = 0;

  return ret;
}

// [TODO] Change to string return so that debug prints are easier to "chain"
void print_rectangle(const Rectangle* rect) {
  printf("[DBG] Rectangle: A1: %f %f\n A2: %f %f\n A3: %f %f\n A4: %f %f\n",
         rect->a1.x, rect->a1.y, rect->a2.x, rect->a2.y, rect->a3.x, rect->a3.y,
         rect->a4.x, rect->a4.y);
}

// Rotates a rectangle by additional n degrees.
// This function rotates "additively", meaning that the parameter angle
// is in essence added to rectangle's rot_angle variable. Therefore,
// rotating by n degrees rotates existing rectangle by 9 degrees,
// not into 0->9 degrees with an assumption of a non-rotated rectangle.
void rotate_rectangle(Rectangle* rectangle, double angle) {
  if (angle == 0) {
    return;
  }

  vec2d rot_center;
  rot_center.x = rectangle->origin_topleft.x + (rectangle->width / 2);
  rot_center.y = rectangle->origin_topleft.y + (rectangle->height / 2);

  rectangle->a1 = rotate_point(rectangle->a1, rot_center, angle);
  rectangle->a2 = rotate_point(rectangle->a2, rot_center, angle);
  rectangle->a3 = rotate_point(rectangle->a3, rot_center, angle);
  rectangle->a4 = rotate_point(rectangle->a4, rot_center, angle);

  rectangle->rot_angle = fmod((rectangle->rot_angle + angle), 360.0);
}

void translate_rectangle(Rectangle* rectangle, vec2d trnsl_magnitude) {
  rectangle->a1.x += trnsl_magnitude.x;
  rectangle->a1.y += trnsl_magnitude.y;

  rectangle->a2.x += trnsl_magnitude.x;
  rectangle->a2.y += trnsl_magnitude.y;

  rectangle->a3.x += trnsl_magnitude.x;
  rectangle->a3.y += trnsl_magnitude.y;

  rectangle->a4.x += trnsl_magnitude.x;
  rectangle->a4.y += trnsl_magnitude.y;

  rectangle->origin_topleft.x += trnsl_magnitude.x;
  rectangle->origin_topleft.y += trnsl_magnitude.y;
}

// Commonly, SAT implementations calculate normals of edges - in our case, as
// the shape is a rectangle, normal of an edge is a neighboring edge. As such,
// we can just take vectors calculated from them.
void _initialize_proj_axes(vec2d proj_axes[4], const Rectangle* r1,
                           const Rectangle* r2) {
  // R1_Pa, axis of A1-A2 edge
  vec2d R1_Pa = vec2d_from_points(r1->a1.x, r1->a1.y, r1->a2.x, r1->a2.y);
  R1_Pa = vec2d_normalize(&R1_Pa);
  proj_axes[0] = R1_Pa;
  // R1_Pb, axis of A2-A4 edge
  vec2d R1_Pb = vec2d_from_points(r1->a2.x, r1->a2.y, r1->a4.x, r1->a4.y);
  R1_Pb = vec2d_normalize(&R1_Pb);
  proj_axes[1] = R1_Pb;

  // R2_Pa, axis of A1-A2 edge
  vec2d R2_Pa = vec2d_from_points(r2->a1.x, r2->a1.y, r2->a2.x, r2->a2.y);
  R2_Pa = vec2d_normalize(&R2_Pa);
  proj_axes[2] = R2_Pa;

  // R2_Pb, axis of A2-A4 edge
  vec2d R2_Pb = vec2d_from_points(r2->a2.x, r2->a2.y, r2->a4.x, r2->a4.y);
  R2_Pb = vec2d_normalize(&R2_Pb);
  proj_axes[3] = R2_Pb;
}

// Function projecting points of a rectangle onto an axis and returning
// the resulting magnitude for collision detection (length on projection axis).
// Projection is provided via passed array proj of two elements, where
// element 0 corresponds to the minimum value and element 1 corresponds
// to the maximum value. Function does not check that the array exists
// and/or is large enough.
void _project_rectangle_vector(const Rectangle* r, const vec2d axis,
                               vec2d projs[2]) {
  vec2d min_vec = {.x = NAN, .y = NAN};
  vec2d max_vec = {.x = NAN, .y = NAN};
  double min_dot = DBL_MAX;
  double max_dot = -DBL_MAX;
  const vec2d* proj_points[4];
  proj_points[0] = &(r->a1);
  proj_points[1] = &(r->a2);
  proj_points[2] = &(r->a3);
  proj_points[3] = &(r->a4);

  for (int i = 0; i < 4; i++) {
    double dot = vec2d_dot_product(proj_points[i], &axis);
    if (dot < min_dot) {
      min_vec = vec2d_project(proj_points[i], &axis);
      min_dot = dot;
    }
    if (dot > max_dot) {
      max_vec = vec2d_project(proj_points[i], &axis);
      max_dot = dot;
    }
  }

  projs[0] = min_vec;
  projs[1] = max_vec;
}

double _point_distance(const vec2d a, const vec2d b) {
  return sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
}

// Function assumes points a, b, btwn are on the same line.
// btwn is between a and b iff |ABTWN| + |BBTWN| == |AB|
int _point_is_between(const vec2d a, const vec2d b, const vec2d btwn) {
  double dist_a_btwn = _point_distance(a, btwn);
  double dist_b_btwn = _point_distance(b, btwn);
  double dist_a_b = _point_distance(a, b);

  if (fabs((dist_a_btwn + dist_b_btwn) - dist_a_b) < 1e-10) {
    return 1;
  }

  return 0;
}

// Function determining whether projections overlap during SAT. Function assumes
// projection points (vectors) are co-directional.
int _projection_overlap(const vec2d vec1[2], const vec2d vec2[2]) {
  // vec2 min further than vec1 min, closer than vec1 max
  if (_point_is_between(vec2[0], vec2[1], vec1[0])) {
    return 1;
  }
  if (_point_is_between(vec1[0], vec1[1], vec2[0])) {
    return 1;
  }

  return 0;
}

vec2d rectangle_center(const Rectangle* rect) {
  vec2d midpoint_top = vec2d_midpoint(&rect->a1, &rect->a2);
  vec2d midpoint_bottom = vec2d_midpoint(&rect->a3, &rect->a4);
  vec2d ret = vec2d_midpoint(&midpoint_top, &midpoint_bottom);

  return ret;
}

/* Implementation of SAT intersection algorithm.
 * Returns 1 if rectangles intersect, 0 otherwise.*/
int rectangle_intersection(const Rectangle* r1, const Rectangle* r2) {
  vec2d proj_axes[4];  // R1_Pa, R1_Pb, R2_Pa, R2_Pb

  _initialize_proj_axes(proj_axes, r1, r2);
  for (int i = 0; i < 4; i++) {
    vec2d curr_axis = proj_axes[i];
    vec2d proj_r1[2];
    _project_rectangle_vector(r1, curr_axis, proj_r1);
    vec2d proj_r2[2];
    _project_rectangle_vector(r2, curr_axis, proj_r2);

    if (!_projection_overlap(proj_r1, proj_r2)) {
      return 0;
    }
  }
  return 1;
}

// ABC test as in
// https://stackoverflow.com/questions/2752725/finding-whether-a-point-lies-inside-a-rectangle-or-not
int point_in_rectangle(const vec2d* point, const Rectangle* rect) {
  vec2d AB, AM, AC;
  AB = vec2d_from_points(rect->a1.x, rect->a1.y, rect->a2.x, rect->a2.y);
  AM = vec2d_from_points(rect->a1.x, rect->a1.y, point->x, point->y);
  AC = vec2d_from_points(rect->a1.x, rect->a1.y, rect->a3.x, rect->a3.y);

  double dot_AB_AM = vec2d_dot_product(&AB, &AM);
  double dot_AB_AB = vec2d_dot_product(&AB, &AB);
  double dot_AM_AC = vec2d_dot_product(&AM, &AC);
  double dot_AC_AC = vec2d_dot_product(&AC, &AC);

  if (0 <= dot_AB_AM && dot_AB_AM <= dot_AB_AB && 0 <= dot_AM_AC &&
      dot_AM_AC <= dot_AC_AC) {
    return 1;
  }

  return 0;
}

// In order to project the points, an axis is used to find two "corners" of a
// circle - those are then projected onto the axis.
// [TODO] Understand dot product properly
void _project_circle_vector(const Circle* circ, const vec2d axis,
                            vec2d projs[2]) {
  vec2d min_vec = {.x = NAN, .y = NAN};
  vec2d max_vec = {.x = NAN, .y = NAN};
  double min_dot = DBL_MAX;
  double max_dot = -DBL_MAX;
  vec2d proj_points[2];

  vec2d direction = vec2d_normalize(&axis);
  vec2d circ_point_1 = {.x = circ->position.x + direction.x * circ->radius,
                        .y = circ->position.y + direction.y * circ->radius};
  direction = vec2d_scalar_multiply(&direction, -1);  // reverse
  vec2d circ_point_2 = {.x = circ->position.x + direction.x * circ->radius,
                        .y = circ->position.y + direction.y * circ->radius};
  proj_points[0] = circ_point_1;
  proj_points[1] = circ_point_2;

  for (int i = 0; i < 2; i++) {
    double dot = vec2d_dot_product(&proj_points[i], &axis);
    if (dot < min_dot) {
      min_vec = vec2d_project(&proj_points[i], &axis);
      min_dot = dot;
    }
    if (dot > max_dot) {
      max_vec = vec2d_project(&proj_points[i], &axis);
      max_dot = dot;
    }
  }

  projs[0] = min_vec;
  projs[1] = max_vec;
}

// Function does not perform a check that rect is not NULL and proj_axes has 2
// elems
void _initialize_proj_axes_rectangle(vec2d proj_axes[2],
                                     const Rectangle* rect) {
  vec2d a1a2 =
      vec2d_from_points(rect->a2.x, rect->a2.y, rect->a1.x, rect->a1.y);
  a1a2 = vec2d_normalize(&a1a2);
  proj_axes[0] = a1a2;

  vec2d a1a3 =
      vec2d_from_points(rect->a3.x, rect->a3.y, rect->a1.x, rect->a1.y);
  a1a3 = vec2d_normalize(&a1a3);
  proj_axes[1] = a1a3;
}

// SAT implementation
int circle_rectangle_intersection(const Circle* circ, const Rectangle* rect) {
  vec2d proj_axes[2];
  _initialize_proj_axes_rectangle(proj_axes, rect);

  for (int i = 0; i < 2; i++) {
    vec2d curr_axis = proj_axes[i];
    vec2d proj_r1[2];
    _project_rectangle_vector(rect, curr_axis, proj_r1);
    vec2d proj_circ[2];
    _project_circle_vector(circ, curr_axis, proj_circ);

    if (!_projection_overlap(proj_r1, proj_circ)) {
      return 0;
    }
  }

  return 1;
}

Circle new_circle(double posX, double posY, double radius) {
  vec2d pos_vec = {.x = posX, .y = posY};

  Circle circle = {.position = pos_vec, .radius = radius};

  return circle;
}

void translate_circle(Circle* circle, vec2d trnsl_magnitude) {
  circle->position.x += trnsl_magnitude.x;
  circle->position.y += trnsl_magnitude.y;
}

// Function does not validate that rect is not NULL and that lines are
// allocated.
void _initialize_edge_lines(Line lines[4], const Rectangle* rect) {
  vec2d a1_a2 =
      vec2d_from_points(rect->a2.x, rect->a2.y, rect->a1.x, rect->a1.y);
  a1_a2 = vec2d_normalize(&a1_a2);
  lines[0] = (Line){.direction = a1_a2, .point = rect->a1};

  vec2d a2_a4 =
      vec2d_from_points(rect->a4.x, rect->a4.y, rect->a2.x, rect->a2.y);
  a2_a4 = vec2d_normalize(&a2_a4);
  lines[1] = (Line){.direction = a2_a4, .point = rect->a2};

  vec2d a1_a3 =
      vec2d_from_points(rect->a3.x, rect->a3.y, rect->a1.x, rect->a1.y);
  a1_a3 = vec2d_normalize(&a1_a3);
  lines[2] = (Line){.direction = a1_a3, .point = rect->a1};

  vec2d a3_a4 =
      vec2d_from_points(rect->a4.x, rect->a4.y, rect->a3.x, rect->a3.y);
  a3_a4 = vec2d_normalize(&a3_a4);
  lines[3] = (Line){.direction = a3_a4, .point = rect->a3};
}

// Intersect line with all axes. If intersection exists, find the point with
// shortest distance
// If no intersection exists, vec2d{NAN, NAN} is returned. Otherwise, vec2d
// representing the point of intersection with shortest distance from original
// point is returned.
// [TODO] Function modifies an array to return point, vector representing line
// direction. Very unobvious behavior, should be done differently.
void find_rect_line_intersection(vec2d intersection[2], const Rectangle* rect,
                                 vec2d point, vec2d direction) {
  vec2d intersection_point = {.x = NAN, .y = NAN};
  double min_mag = DBL_MAX;
  Line edge_lines[4];
  _initialize_edge_lines(edge_lines, rect);
  Line movement_line = {.point = point, .direction = direction};
  vec2d edge_vec = {.x = NAN, .y = NAN};
  for (int i = 0; i < 4; i++) {
    vec2d tmp =
        line_intersection(&movement_line.direction, &movement_line.point,
                          &edge_lines[i].direction, &edge_lines[i].point);
    if (!isnan(tmp.x)) {
      vec2d point_to_intersection =
          vec2d_from_points(tmp.x, tmp.y, point.x, point.y);
      double intersection_mag = vec2d_magnitude(&point_to_intersection);
      if (intersection_mag < min_mag) {
        min_mag = intersection_mag;
        intersection_point = tmp;
        edge_vec = edge_lines[i].direction;
      }
    }
  }

  intersection[0] = intersection_point;
  intersection[1] = edge_vec;
}

// Returns directions of rectangle's sides. Returned axes correspond to a1->a2
// and a1->a3. Directions are normalized vectors. The function does not perform
// checks that rect == NULL and axes has len >= 2.
void rectangle_axes(vec2d axes[2], const Rectangle* rect) {
  vec2d a1a2 =
      vec2d_from_points(rect->a2.x, rect->a2.y, rect->a1.x, rect->a1.y);
  a1a2 = vec2d_normalize(&a1a2);

  vec2d a1a3 =
      vec2d_from_points(rect->a3.x, rect->a3.y, rect->a1.x, rect->a1.y);
  a1a3 = vec2d_normalize(&a1a3);

  axes[0] = a1a2;
  axes[1] = a1a3;
}
