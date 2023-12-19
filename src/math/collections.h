#ifndef _COLLECTIONS_H_
#define _COLLECTIONS_H_
#include "shapes.h"

typedef struct RectangleArray {
  Rectangle* array;
  unsigned int size;
  unsigned int capacity;
} RectangleArray;

RectangleArray* new_rectangle_array(unsigned int capacity);
void destroy_rectangle_array(RectangleArray* r_arr);
int rectanglearray_insert_rectangle(RectangleArray* r_arr,
                                    const Rectangle* rect);

#endif  // _COLLECTIONS_H_
