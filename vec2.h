#ifndef VEC2_H
#define VEC2_H

#define TVec float

typedef struct s_vec2
{
    TVec x, y;
} Vec2;

// test if a and b are equals (exactly the same x and y values)
int V2_equal(Vec2 a, Vec2 b);

// vector initialisation
Vec2 V2_init(TVec x, TVec y);

// vector addition
Vec2 V2_add(Vec2 a, Vec2 b);

// vector substraction
Vec2 V2_minus(Vec2 a, Vec2 b);

// dot product between two a and b
float V2_dot(Vec2 a, Vec2 b);

// cross product between a and b
float V2_cross(Vec2 a, Vec2 b);

// multiplication by a factor
Vec2 V2_mulFac(Vec2 a, float f);

// IMPLICIT TYPE CONVERSION
// division by a factor
Vec2 V2_divFac(Vec2 a, float f);

// vector length squared (optimized for comparison)
float V2_lengthSq(Vec2 a);

// true vector length
float V2_length(Vec2 a);

// squared distance between two vector
float V2_distanceSq(Vec2 a, Vec2 b);

// true distance between two vectors
float V2_distance(Vec2 a, Vec2 b);

#endif