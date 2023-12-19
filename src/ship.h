#ifndef _SHIP_H_
#define _SHIP_H_
#include <stdlib.h>

#include "math/shapes.h"
#include "math/vector.h"

// [TODO] To consider moving movement direction/shooting direction vectors into
// the ship struct
typedef struct Ship {
  vec2d position;           // topleft origin to place texture to
  unsigned int ship_width;  // Constraint - player ship/texture is square-shaped
  Circle hurtcirc;
  vec2d hurtcirc_offset;  // offset from position where hurtbox is to be placed
  Circle collision_circ;
  vec2d collision_circ_offset;
} Ship;

Ship create_ship(const vec2d position, size_t ship_width,
                 const double hurtcirc_radius, const vec2d* hurtcirc_offset,
                 const double* collcirc_radius, const vec2d* collcirc_offset);
void update_ship(Ship* ship, const vec2d movement);
void move_ship_by_collision(Ship* ship, const vec2d target);
void relocate_ship(Ship* ship, const vec2d pos);

#endif  // _SHIP_H_
