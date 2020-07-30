#include <algorithm>
#include <tuple>
#include <vector>
#include "Rasterizer.h"
#include "Renderer.h"

Point *ScalePoint(const Point *p)
{
    return new Point{p->x * SAMPLE_SCALE, p->y * SAMPLE_SCALE};
}

void ScalePointByref(Point *p)
{
    p->x = p->x * SAMPLE_SCALE;
    p->y = p->y * SAMPLE_SCALE;
}

Triangle *ScaleTriangle(const Triangle *triangle)
{
    Triangle *scaled = new Triangle{triangle->a, triangle->b, triangle->c};
    ScalePointByref(&scaled->a);
    ScalePointByref(&scaled->b);
    ScalePointByref(&scaled->c);
    return scaled;
}

void DrawPixel(int x, int y, uint32_t color)
{
    pixels[(PLANE_H * SAMPLE_SCALE) - y - 1][x] = color;
}

void DrawLine(const Point *p, const Point *q, uint32_t color)
{
    const Point *left = p->x < q->x ? p : q;
    const Point *right = p->x < q->x ? q : p;
    
    left = ScalePoint(left);
    right = ScalePoint(right);

    int dy = right->y - left->y;
    int dx = right->x - left->x;

    if (dx == 0) {
        int min_y = std::min(left->y, right->y);
        for (int i = 0; i < std::abs(dy); i++) {
            DrawPixel(left->x, min_y + i, color);
        }
        return;
    }

    float m = (float)dy / (float)dx;
    int m_sign = m >= 0 ? 1 : -1;

    for (int x = 0; x < dx; x++) {
        float rel_y = m * x;
        int abs_x = left->x + x;
        int abs_y = left->y + rel_y;

        int next_y = (int)(left->y + (m * (x + 1)));

        for (int y = abs_y; y != next_y; y += m_sign) {
            DrawPixel(abs_x, y, color);
        }

        DrawPixel(abs_x, abs_y, color);
    }

    delete left;
    delete right;
}


// Naive approach to scanline filling which traverses the grid in which the triangle lies, checking if every point is within the triangle.
// @TODO: Have to make this function work again.
// void FillTriangleNaive(const Triangle &triangle)
// {
//     int min_x = std::min(std::min(triangle.a.x, triangle.b.x), triangle.c.x);
//     int min_y = std::min(std::min(triangle.a.y, triangle.b.y), triangle.c.y);
//     int max_x = std::max(std::max(triangle.a.x, triangle.b.x), triangle.c.x);
//     int max_y = std::max(std::max(triangle.a.y, triangle.b.y), triangle.c.y);

//     for (int i = min_y; i < max_y; i++) {
//         for (int j = min_x; j < max_x; j++) {
//             if (PointInTriangle(Point{j, i}, triangle)) {
//                 DrawPixel(j, i, 1);
//             }
//         }
//     }
// }

void FillFlatBaseTriangleHelper(const Point *a, const Point *b, const Point *c, uint32_t color)
{
    float m_ab = 0, m_ac = 0, b_ab = 0, b_ac = 0;

    a = ScalePoint(a);
    b = ScalePoint(b);
    c = ScalePoint(c);

    bool ab_vert = a->x == b->x;
    bool ac_vert = a->x == c->x;

    if (!ab_vert) {
        m_ab = (float)(a->y - b->y) / (float)(a->x - b->x);
        b_ab = a->y - (m_ab * a->x);
    }

    if (!ac_vert) {
        m_ac = (float)(a->y - c->y) / (float)(a->x - c->x);
        b_ac = a->y - (m_ac * a->x);
    }

    int direction = a->y < b->y ? 1 : -1;

    for (int i = a->y; i != b->y; i += direction) {
        int j = !ab_vert ? (i - b_ab) / m_ab : a->x;
        int k = !ac_vert ? (i - b_ac) / m_ac : a->x;

        for (int x = std::min(j, k); x <= std::max(j, k); x++) {
            DrawPixel(x, i, color);
        }
    }

    delete a;
    delete b;
    delete c;
}

void FillFlatBottomTriangle(const Triangle *triangle, uint32_t color) 
{
    FillFlatBaseTriangleHelper(&triangle->a, &triangle->b, &triangle->c, color);
}

void FillFlatTopTriangle(const Triangle *triangle, uint32_t color)
{
    FillFlatBaseTriangleHelper(&triangle->c, &triangle->a, &triangle->b, color);
}

// Custom triangle filling algorithm that apparently works very similarly to standard triangle filling algorithms / scanline based rendering (except we're using a single shape here).
void FillTriangleOpt(const Triangle *triangle, uint32_t color)
{
    const Point *a, *b, *c;

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

void DrawTriangle(const Triangle *triangle, uint32_t color, bool fill = false)
{
    if (fill) {
        FillTriangleOpt(triangle, color);
    } else {
        DrawLine(&triangle->a, &triangle->b, color);
        DrawLine(&triangle->b, &triangle->c, color);
        DrawLine(&triangle->c, &triangle->a, color);
    }
}
