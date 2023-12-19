#include "collections.h"

#include <stdio.h>
#include <stdlib.h>

RectangleArray* new_rectangle_array(unsigned int capacity) {
  if (capacity == 0) {
    printf("[ERROR] Attempted to allocate rectarray with cap 0\n");
    return NULL;
  }

  RectangleArray* ret = malloc(sizeof(RectangleArray));
  if (ret == NULL) {
    printf("[ERROR] Error allocating a new rectarray, cap %u\n", capacity);
    return NULL;
  }

  Rectangle* arr = malloc(sizeof(Rectangle) * capacity);
  if (arr == NULL) {
    free(ret);
    printf("[ERROR] Error allocating a new rect array, cap %u\n", capacity);
    return NULL;
  }

  ret->array = arr;
  ret->size = 0;
  ret->capacity = capacity;

  return ret;
}

void destroy_rectangle_array(RectangleArray* r_arr) {
  free(r_arr->array);
  free(r_arr);
}

// No dynamic resizing is implemented. If element can't be inserted, 0 is
// returned.
// [TODO] No reason for Rect to be a pointer - just becomes confusing
int rectanglearray_insert_rectangle(RectangleArray* r_arr,
                                    const Rectangle* rect) {
  if (r_arr->size == r_arr->capacity) {
    printf(
        "[WARN] Attempted to insert a rectangle into a rect array over "
        "capacity\n");
    return 0;
  }

  Rectangle new = *rect;
  r_arr->array[r_arr->size] = new;
  r_arr->size += 1;

  return 1;
}
