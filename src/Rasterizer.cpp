#include <algorithm>
#include <tuple>
#include <vector>
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

void FillFlatBottomTriangle(Triangle *triangle, uint32_t color)
{
    float m_ab = 0, m_ac = 0, b_ab = 0, b_ac = 0;

    bool ab_vert = triangle->a.x == triangle->b.x;
    bool ac_vert = triangle->a.x == triangle->c.x;

    if (!ab_vert) {
        m_ab = (float)(triangle->a.y - triangle->b.y) / (float)(triangle->a.x - triangle->b.x);
        b_ab = triangle->a.y - (m_ab * triangle->a.x);
    }

    if (!ac_vert) {
        m_ac = (float)(triangle->a.y - triangle->c.y) / (float)(triangle->a.x - triangle->c.x);
        b_ac = triangle->a.y - (m_ac * triangle->a.x);
    }

    for (int i = triangle->a.y; i > triangle->b.y; i--) {
        int j = !ab_vert ? (i - b_ab) / m_ab : triangle->a.x;
        int k = !ac_vert ? (i - b_ac) / m_ac : triangle->a.x;

        for (int x = std::min(j, k); x <= std::max(j, k); x++) {
            pixels[PLANE_H - i - 1][x] = color;
        }
    }
}

void FillFlatTopTriangle(Triangle *triangle, uint32_t color)
{
    float m_ac = 0, m_bc = 0, b_ac = 0, b_bc = 0;

    bool ac_vert = triangle->a.x == triangle->c.x;
    bool bc_vert = triangle->b.x == triangle->c.x;

    if (!ac_vert) {
        m_ac = (float)(triangle->a.y - triangle->c.y) / (float)(triangle->a.x - triangle->c.x);
        b_ac = triangle->a.y - (m_ac * triangle->a.x);
    }
    
    if (!bc_vert) {
        m_bc = (float)(triangle->b.y - triangle->c.y) / (float)(triangle->b.x - triangle->c.x);
        b_bc = triangle->b.y - (m_bc * triangle->b.x);
    }

    for (int i = triangle->b.y; i > triangle->c.y; i--) {
        int j = !ac_vert ? (i - b_ac) / m_ac : triangle->c.x;
        int k = !bc_vert ? (i - b_bc) / m_bc : triangle->c.x;

        for (int x = std::min(j, k); x <= std::max(j, k); x++) {
            pixels[PLANE_H - i - 1][x] = color;
        }
    }
}

// Custom triangle filling algorithm that apparently works very similarly to standard triangle filling algorithms / scanline based rendering (except we're using a single shape here).
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

    Triangle ordered{*a, *b, *c};

    if (a->y == b->y) {
        FillFlatTopTriangle(&ordered, color);
    } else if (b->y == c->y) {
        FillFlatBottomTriangle(&ordered, color);
    } else {
        int x = a->x;

        if (a->x - c->x != 0) {
            float m_ac = (float)(a->y - c->y) / (float)(a->x - c->x);
            float b_ac = a->y - (m_ac * a->x);
            x = (b->y - b_ac) / m_ac;
        }

        Point d{x, b->y};

        Triangle top{*a, *b, d};
        Triangle bot{d, *b, *c};

        FillFlatBottomTriangle(&top, color);
        FillFlatTopTriangle(&bot, color);
    }
}

void DrawTriangle(Triangle *triangle, uint32_t color, bool fill = false)
{
    if (fill) {
        FillTriangleOpt(triangle, color);
    } else {
        DrawLine(triangle->a, triangle->b, color);
        DrawLine(triangle->b, triangle->c, color);
        DrawLine(triangle->c, triangle->a, color);
    }
}
