#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <math.h>

#include "vec2.h"

// sort the array T and perform the same operations on array S
// bubble sort
void sort(int *T, int *S, size_t size);

// swap a and b values
void swap(int *a, int *b);

// compute the intersection point between two segments
// if the intersection exists, the function returns 1 and store the
//   result in I
// else the function return 0
int intersectPoint(Vec2 a, Vec2 u, Vec2 b, Vec2 v, Vec2 *I);

// compute the distance between p and the segment ab (see readme.md)
float distanceFromSegment(Vec2 a, Vec2 b, Vec2 p);

#endif