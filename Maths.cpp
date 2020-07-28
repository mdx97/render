#include <cmath>
#include "Maths.h"

float TriangleArea(const Triangle &triangle)
{
    return std::fabs((
        (triangle.a.x * (triangle.b.y - triangle.c.y)) + 
        (triangle.b.x * (triangle.c.y - triangle.a.y)) + 
        (triangle.c.x * (triangle.a.y - triangle.b.y))) / 2);
}

bool PointInTriangle(const Point &point, const Triangle &triangle)
{
    Triangle component_triangles[3];
    
    component_triangles[0].a = point;
    component_triangles[0].b = triangle.a;
    component_triangles[0].c = triangle.b;

    component_triangles[1].a = point;
    component_triangles[1].b = triangle.b;
    component_triangles[1].c = triangle.c;

    component_triangles[2].a = point;
    component_triangles[2].b = triangle.a;
    component_triangles[2].c = triangle.c;

    float area = 0;

    for (Triangle component : component_triangles) {
        area += TriangleArea(component);
    }

    return area <= TriangleArea(triangle);
}
