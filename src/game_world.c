#include "game_world.h"

#include <stdio.h>
#include <stdlib.h>

#include "constants.h"

// Boundaries are implemented as 4 rectangles wrapped around the screen
// [TODO] Validate return values on insertion
// [TODO] failure, rects would need to be free'd
void create_game_boundaries(RectangleArray* rect_arr) {
  // left
  double posX = -50;
  double posY = -50;
  double width = 70;
  double height = SCREEN_HEIGHT + 200;
  Rectangle left_boundary = new_rectangle(posX, posY, width, height);
  rectanglearray_insert_rectangle(rect_arr, &left_boundary);
  //  top
  width = SCREEN_WIDTH + 200;
  height = 70;
  Rectangle top_boundary = new_rectangle(posX, posY, width, height);
  rectanglearray_insert_rectangle(rect_arr, &top_boundary);
  //   bottom
  posY = SCREEN_HEIGHT - 50;
  Rectangle bottom_boundary = new_rectangle(posX, posY, width, height);
  rectanglearray_insert_rectangle(rect_arr, &bottom_boundary);
  //  right
  posX = SCREEN_WIDTH - 50;
  posY = -50;
  width = 70;
  height = SCREEN_HEIGHT + 200;
  Rectangle right_boundary = new_rectangle(posX, posY, width, height);
  rectanglearray_insert_rectangle(rect_arr, &right_boundary);
}

GameWorld* new_game_world(void) {
  RectangleArray* rect_arr = new_rectangle_array(300);
  if (rect_arr == NULL) {
    printf("[ERROR] malloc error rect array in new_game_world\n");
    return NULL;
  }

  Rectangle test_rect = new_rectangle(90, 90, 40, 120);
  Rectangle test_rect2 = new_rectangle(240, 300, 120, 40);
  rectanglearray_insert_rectangle(rect_arr, &test_rect);
  rectanglearray_insert_rectangle(rect_arr, &test_rect2);

  Rectangle nested1 = new_rectangle(100, 100, 120, 40);
  Rectangle nested2 = new_rectangle(120, 120, 60, 60);
  rectanglearray_insert_rectangle(rect_arr, &nested1);
  rectanglearray_insert_rectangle(rect_arr, &nested2);

  GameWorld* ret = malloc(sizeof(GameWorld));
  if (ret == NULL) {
    printf("[ERROR] malloc error gameworld in new_game_world\n");
    destroy_rectangle_array(rect_arr);
    return NULL;
  }

  create_game_boundaries(rect_arr);

  ret->rects = rect_arr;

  return ret;
}

void destroy_game_world(GameWorld* world) {
  if (world != NULL) {
    destroy_rectangle_array(world->rects);
    free(world);
  }
}
