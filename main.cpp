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

void FillTriangleNaive(const Triangle &triangle)
{
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

void FillTriangleOpt(Triangle *triangle)
{
    Point *a, *b, *c;

    a = &triangle->a;
    if (triangle->b.y > a->y) a = &triangle->b;
    if (triangle->c.y > a->y) a = &triangle->c;

    c = &triangle->a;
    if (triangle->b.y < c->y) c = &triangle->b;
    if (triangle->c.y < c->y) c = &triangle->c;

    if (&triangle->a != a && &triangle->a != c) b = &triangle->a;
    else if (&triangle->b != a && &triangle->b != c) b = &triangle->b;
    else if (&triangle->c != a && &triangle->c != c) b = &triangle->c;

    float m_ab = (float)(a->y - b->y) / (float)(a->x - b->x);
    float m_ac = (float)(a->y - c->y) / (float)(a->x - c->x);
    float m_bc = (float)(b->y - c->y) / (float)(b->x - c->x);

    float b_ab = a->y - (m_ab * a->x);
    float b_ac = a->y - (m_ac * a->x);
    float b_bc = b->y - (m_bc * b->x);

    int j;

    for (int i = a->y; i > c->y; i--) {
        if (i > b->y) j = (i - b_ab) / m_ab;
        else          j = (i - b_bc) / m_bc;

        int k = (i - b_ac) / m_ac;

        for (int z = std::min(j, k); z < std::max(j, k); z++) {
            pixels[PLANE_H - i - 1][z] = 1;
        }
    }
}

void DrawTriangle(Triangle *triangle, bool fill)
{
    DrawLine(triangle->a, triangle->b);
    DrawLine(triangle->b, triangle->c);
    DrawLine(triangle->c, triangle->a);

    if (fill) FillTriangleOpt(triangle);
}

int main()
{
    std::memset(pixels, 0, sizeof(pixels));

    //Triangle triangle{{1, 40}, {60, 80}, {120, 50}};
    //Triangle triangle {{80, 1}, {41, 80}, {1, 50}};
    Triangle triangle {{1, 40}, {41, 80}, {120, 1}};
    DrawTriangle(&triangle, true);

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
