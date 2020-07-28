#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>

#define PLANE_W 128
#define PLANE_H 128

typedef struct point_t {
    int x;
    int y;
} Point;


typedef struct triangle_t {
    Point a;
    Point b;
    Point c;
} Triangle;

int pixels[PLANE_H][PLANE_W];

void DrawLine(const Point &p, const Point &q)
{
    const Point &left = p.x < q.x ? p : q;
    const Point &right = p.x < q.x ? q : p;
    
    int rise = right.y - left.y;
    int run = right.x - left.x;

    if (run == 0) {
        int min_y = std::min(left.y, right.y);
        for (int i = 0; i < std::abs(rise); i++) {
            pixels[PLANE_H - min_y - i - 1][left.x] = 1;
        }
        return;
    }

    float slope = (float)rise / (float)run;
    int slope_sign = slope >= 0 ? 1 : -1;

    for (int x = 0; x < run; x++) {
        float rel_y = slope * x;
        int abs_x = left.x + x;
        int abs_y = left.y + rel_y;

        pixels[PLANE_H - abs_y - 1][abs_x] = 1;

        int next_y = (int)(left.y + (slope * (x + 1)));

        for (int y = abs_y; y != next_y; y += slope_sign) {
            pixels[PLANE_H - y - 1][abs_x] = 1;
        }
    }
}

float TriangleArea(const Triangle &triangle)
{
    return std::fabs(((triangle.a.x * (triangle.b.y - triangle.c.y)) + (triangle.b.x * (triangle.c.y - triangle.a.y)) + (triangle.c.x * (triangle.a.y - triangle.b.y))) / 2);
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

void DrawTriangle(const Triangle &triangle, bool fill)
{
    DrawLine(triangle.a, triangle.b);
    DrawLine(triangle.b, triangle.c);
    DrawLine(triangle.c, triangle.a);

    if (fill) {
        int min_x = std::min(std::min(triangle.a.x, triangle.b.x), triangle.c.x);
        int min_y = std::min(std::min(triangle.a.y, triangle.b.y), triangle.c.y);
        int max_x = std::max(std::max(triangle.a.x, triangle.b.x), triangle.c.x);
        int max_y = std::max(std::max(triangle.a.y, triangle.b.y), triangle.c.y);

        for (int i = min_y; i < max_y; i++) {
            for (int j = min_x; j < max_x; j++) {
                if (PointInTriangle(Point{j, i}, triangle)) {
                    pixels[PLANE_H - i - 1][j] = 1;
                }
            }
        }
    }
}

int main()
{
    std::memset(pixels, 0, sizeof(pixels));

    Triangle triangle {{1, 40}, {41, 80}, {120, 0}};
    DrawTriangle(triangle, true);

    for (int i = 0; i < PLANE_H; i++) {
        for (int j = 0; j < PLANE_W; j++) {
            if (pixels[i][j] != 0) {
                std::cout << pixels[i][j] << " ";
            } else {
                std::cout << "  ";
            }
        }
        std::cout << std::endl;
    }

    return 0;
}
