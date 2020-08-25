#include <math.h>

#define ALPHA   0.00001

#include "vec2.h"

int V2_equal(Vec2 a, Vec2 b)
{
    if(abs(a.x - b.x) < ALPHA && abs(a.y-b.y) < ALPHA )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

Vec2 V2_init(TVec x, TVec y)
{
    Vec2 v = {x, y};
    return v;
}

Vec2 V2_add(Vec2 a, Vec2 b)
{
    Vec2 v = {a.x + b.x, a.y + b.y};
    return v;
}

Vec2 V2_minus(Vec2 a, Vec2 b)
{
    Vec2 v = {a.x - b.x, a.y - b.y};
    return v;
}

float V2_dot(Vec2 a, Vec2 b)
{
    return a.x * b.x + a.y * b.y;
}

float V2_cross(Vec2 a, Vec2 b)
{
    return a.x * b.y - a.y * b.x;
}

Vec2 V2_mulFac(Vec2 a, float f)
{
    Vec2 v = {a.x * f, a.y * f};
    return v;
}

Vec2 V2_divFac(Vec2 a, float f)
{
    if (f != 0)
    {
        Vec2 v = {a.x / f, a.y / f};
        return v;
    }
    else
    {
        return a;
    }
}

float V2_lengthSq(Vec2 a)
{
    return a.x * a.x + a.y * a.y;
}

float V2_length(Vec2 a)
{
    return sqrt(a.x * a.x + a.y * a.y);
}

float V2_distanceSq(Vec2 a, Vec2 b)
{
    return V2_lengthSq(V2_minus(b,a));
}

float V2_distance(Vec2 a, Vec2 b)
{
    return V2_length(V2_minus(b,a));
}