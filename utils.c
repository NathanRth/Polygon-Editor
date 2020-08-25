#include "utils.h"

// bubble sort
void sort(int *T, int *S, size_t size)
{
    if (size > 1)
    {
        for (size_t i = size - 1; i >= 1; i--)
        {
            for (size_t j = 0; j < i; j++)
            {
                if (T[j + 1] < T[j])
                {
                    swap(&T[j + 1], &T[j]);
                    swap(&S[j + 1], &S[j]);
                }
            }
        }
    }
}

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int intersectPoint(Vec2 A, Vec2 B, Vec2 C, Vec2 D, Vec2 *I)
{
    float ABx = (B.x - A.x);
    float ABy = (B.y - A.y);
    float DCx = (C.x - D.x);
    float DCy = (C.y - D.y);

    float denom = ABx * DCy - DCx * ABy;

    if (denom != 0)
    {
        float CAx = (A.x - C.x);
        float CAy = (A.y - C.y);

        float s = (DCx * CAy - CAx * DCy) / denom;
        float t = (CAx * ABy - ABx * CAy) / denom;

        if ((s >= 0.0) && (s <= 1.0) && (t >= 0.0) && (t <= 1.0))
        {
            *I = V2_add(C, V2_mulFac(V2_minus(D, C), t));
            return 1;
        }
    }
    return 0;
}

// where c is the mouse position
float distanceFromSegment(Vec2 A, Vec2 B, Vec2 C)
{
    Vec2 AB = V2_minus(B, A);
    Vec2 AC = V2_minus(C, A);

    float t = V2_dot(AC, AB) / V2_lengthSq(AB);

    // if the projection is not between A and B
    // let t correspond to the closest point
    if (t < 0 || t > 1)
    {
        t = fmin(fmax(0, t), 1);
    }

    Vec2 proj = V2_add(A, V2_mulFac(AB, t));

    // return the square distance between the mouse position
    // and the projection. As the distance will be used for
    // comparison only, it is not necessary to srqt the result
    return V2_distanceSq(proj, C);
}