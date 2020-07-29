#include <algorithm>
#include "Rasterizer.h"
#include "Renderer.h"

void DrawLine(const Point &p, const Point &q, uint32_t color)
{
    const Point &left = p.x < q.x ? p : q;
    const Point &right = p.x < q.x ? q : p;
    
    int dy = right.y - left.y;
    int dx = right.x - left.x;

    if (dx == 0) {
        int min_y = std::min(left.y, right.y);
        for (int i = 0; i < std::abs(dy); i++) {
            pixels[PLANE_H - min_y - i - 1][left.x] = color;
        }
        return;
    }

    float m = (float)dy / (float)dx;
    int m_sign = m >= 0 ? 1 : -1;

    for (int x = 0; x < dx; x++) {
        float rel_y = m * x;
        int abs_x = left.x + x;
        int abs_y = left.y + rel_y;

        int next_y = (int)(left.y + (m * (x + 1)));

        for (int y = abs_y; y != next_y; y += m_sign) {
            pixels[PLANE_H - y - 1][abs_x] = color;
        }

        pixels[PLANE_H - abs_y - 1][abs_x] = color;
    }
}


// Naive approach to scanline filling which traverses the grid in which the triangle lies, checking if every point is within the triangle.
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

// Custom triangle filling algorithm that apparently works very similarly to standard triangle filling algorithms / scanline based rendering (except we're using a single shape here).
// TODO: Needs to accomodate flat topped and flat bottomed triangles.
void FillTriangleOpt(Triangle *triangle, uint32_t color)
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

    for (int i = a->y; i > c->y; i--) {
        int j = (i > b->y) ?
                (i - b_ab) / m_ab :
                (i - b_bc) / m_bc;

        int k = (i - b_ac) / m_ac;

        for (int x = std::min(j, k); x <= std::max(j, k); x++) {
            pixels[PLANE_H - i - 1][x] = color;
        }
    }
}


void DrawTriangle(Triangle *triangle, uint32_t color, bool fill = false)
{
    // TODO: Need to test the triangle filling without these calls. In theory, we shouldn't need to draw the individual lines.
    DrawLine(triangle->a, triangle->b, color);
    DrawLine(triangle->b, triangle->c, color);
    DrawLine(triangle->c, triangle->a, color);

    if (fill)
        FillTriangleOpt(triangle, color);
}
