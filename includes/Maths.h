#ifndef MATHS_H
#define MATHS_H

typedef struct point_t {
    int x;
    int y;
} Point;

typedef struct triangle_t {
    Point a;
    Point b;
    Point c;
} Triangle;

float TriangleArea(const Triangle &triangle);
bool PointInTriangle(const Point &point, const Triangle &triangle);

#endif
