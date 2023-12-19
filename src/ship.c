#include "ship.h"

#include "math/vector.h"
vec2d _calc_ship_center_offset(size_t ship_width) {
  vec2d ret = {.x = ship_width / 2.0, .y = ship_width / 2.0};
  return ret;
}

// Function creating a ship. Ship is represented as a square, position
// represented via the topleft corner. Hurtbox and collision box are by default
// calculated as being in the center of such square, which is provided by
// passing NULL. If those should be elsewhere, a specific offset should be
// passed.
Ship create_ship(const vec2d position, size_t ship_width,
                 const double hurtcirc_radius, const vec2d* hurtcirc_offset,
                 const double* collcirc_radius, const vec2d* collcirc_offset) {
  double collcirc_radius_stack;  // workaround as a pointer is passed
  vec2d hurtcirc_offset_stack;
  vec2d collcirc_offset_stack;

  if (hurtcirc_offset == NULL) {
    hurtcirc_offset_stack = _calc_ship_center_offset(ship_width);
  } else {
    hurtcirc_offset_stack = *hurtcirc_offset;
  }
  if (collcirc_radius == NULL) {
    collcirc_radius_stack = ship_width / 2.0;
  } else {
    collcirc_radius_stack = *collcirc_radius;
  }
  if (collcirc_offset == NULL) {
    collcirc_offset_stack = _calc_ship_center_offset(ship_width);
  } else {
    collcirc_offset_stack = *collcirc_offset;
  }

  vec2d hurtcirc_position = {.x = position.x + hurtcirc_offset_stack.x,
                             .y = position.y + hurtcirc_offset_stack.y};
  vec2d collcirc_position = {.x = position.x + collcirc_offset_stack.y,
                             .y = position.y + collcirc_offset_stack.y};

  Circle hurtcircle = {.position = hurtcirc_position,
                       .radius = hurtcirc_radius};
  Circle collcirc = {.position = collcirc_position,
                     .radius = collcirc_radius_stack};

  Ship ret = {.position = position,
              .ship_width = ship_width,
              .hurtcirc = hurtcircle,
              .hurtcirc_offset = hurtcirc_offset_stack,
              .collision_circ = collcirc,
              .collision_circ_offset = collcirc_offset_stack};

  return ret;
}

// Utility function moving a ship by it's collision circle. The function
// receives a center position for collision circle and based on offsets defined
// moves the ship position as well as the hurt circle.
void move_ship_by_collision(Ship* ship, const vec2d target) {
  ship->collision_circ.position = target;
  ship->position.x =
      ship->collision_circ.position.x - ship->collision_circ_offset.x;
  ship->position.y =
      ship->collision_circ.position.y - ship->collision_circ_offset.y;

  ship->hurtcirc.position.x = ship->position.x + ship->hurtcirc_offset.x;
  ship->hurtcirc.position.y = ship->position.y + ship->hurtcirc_offset.y;
}

// [TODO] To consider having ship velocity as a ship attribute rather than
// player's
void update_ship(Ship* ship, const vec2d movement) {
  ship->position.x += movement.x;
  ship->position.y += movement.y;

  ship->hurtcirc.position.x += movement.x;
  ship->hurtcirc.position.y += movement.y;

  ship->collision_circ.position.x += movement.x;
  ship->collision_circ.position.y += movement.y;
}

// Utility function relocating a ship. Typically used for respawning after
// a ship has been destroyed.
void relocate_ship(Ship* ship, const vec2d pos) {
  vec2d hurtcirc_position = {.x = pos.x + ship->hurtcirc_offset.x,
                             .y = pos.y + ship->hurtcirc_offset.y};
  vec2d collcirc_position = {.x = pos.x + ship->collision_circ_offset.x,
                             .y = pos.y + ship->collision_circ_offset.y};

  ship->position = pos;
  ship->hurtcirc.position = hurtcirc_position;
  ship->collision_circ.position = collcirc_position;
}
