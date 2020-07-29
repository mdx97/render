#ifndef MATHS_H
#define MATHS_H

#define PI 3.14159265358979323846

typedef struct point_t {
    int x;
    int y;
} Point;

typedef struct triangle_t {
    Point a;
    Point b;
    Point c;
} Triangle;

inline double d2r(double degrees)
{
    return 2 * PI * (degrees / 360);
}

float TriangleArea(const Triangle &triangle);
bool PointInTriangle(const Point &point, const Triangle &triangle);

#endif
