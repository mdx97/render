#include <algorithm>
#include <tuple>
#include <vector>
#include "Rasterizer.h"
#include "Renderer.h"

// Allocates a new scaled version of the given point. (with respect to the current AA sample scale.)
Point *ScalePoint(const Point *p)
{
    return new Point{p->x * SAMPLE_SCALE, p->y * SAMPLE_SCALE};
}

// Scales the given point. (with respect to the current AA sample scale.)
void ScalePointByref(Point *p)
{
    p->x = p->x * SAMPLE_SCALE;
    p->y = p->y * SAMPLE_SCALE;
}

// Scales the given triangle. (with respect to the current AA sample scale.)
Triangle *ScaleTriangle(const Triangle *triangle)
{
    Triangle *scaled = new Triangle{triangle->a, triangle->b, triangle->c};
    ScalePointByref(&scaled->a);
    ScalePointByref(&scaled->b);
    ScalePointByref(&scaled->c);
    return scaled;
}

// Draws a pixel at the *scaled* coordinate x, y.
void DrawPixel(int x, int y, uint32_t color)
{
    pixels[(PLANE_H * SAMPLE_SCALE) - y - 1][x] = color;
}

// Draws a line between two points with respect to the *unscaled* plane.
void DrawLine(const Point *p, const Point *q, uint32_t color)
{
    const Point *left = p->x < q->x ? p : q;
    const Point *right = p->x < q->x ? q : p;
    
    left = ScalePoint(left);
    right = ScalePoint(right);

    int dy = right->y - left->y;
    int dx = right->x - left->x;

    if (dx == 0) {
        // Line is vertical, so we don't need to worry about moving horizontally through the grid, as the code below does.
        int min_y = std::min(left->y, right->y);
        for (int rel_y = 0; rel_y < std::abs(dy); rel_y++) {
            DrawPixel(left->x, min_y + rel_y, color);
        }
        return;
    }

    float m = static_cast<float>(dy) / static_cast<float>(dx);
    int m_sign = m >= 0 ? 1 : -1;

    for (int rel_x = 0; rel_x < dx; rel_x++) {
        float rel_y = m * rel_x;
        
        int abs_x = left->x + rel_x;
        int abs_y = left->y + rel_y;

        int next_y = static_cast<int>(left->y + (m * (rel_x + 1)));

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

// Fills a triangle (consisting of the given points) that meets the following constraints.
// a.y != b.y = c.y
void FillFlatBaseTriangleHelper(const Point *a, const Point *b, const Point *c, uint32_t color)
{
    float m_ab = 0, m_ac = 0, b_ab = 0, b_ac = 0;

    a = ScalePoint(a);
    b = ScalePoint(b);
    c = ScalePoint(c);

    bool ab_vert = a->x == b->x;
    bool ac_vert = a->x == c->x;

    if (!ab_vert) {
        m_ab = static_cast<float>(a->y - b->y) / static_cast<float>(a->x - b->x);
        b_ab = a->y - (m_ab * a->x);
    }

    if (!ac_vert) {
        m_ac = static_cast<float>(a->y - c->y) / static_cast<float>(a->x - c->x);
        b_ac = a->y - (m_ac * a->x);
    }

    int direction = a->y < b->y ? 1 : -1;

    for (int y = a->y; y != b->y + direction; y += direction) {
        // Solve for x for the two triangle sides at our current y.
        // This gives us the two x coordinates that we need to fill in this row of pixels.
        int x1 = !ab_vert ? (y - b_ab) / m_ab : a->x;
        int x2 = !ac_vert ? (y - b_ac) / m_ac : a->x;

        for (int x = std::min(x1, x2); x <= std::max(x1, x2); x++) {
            DrawPixel(x, y, color);
        }
    }

    delete a;
    delete b;
    delete c;
}

// Fills a triangle that meets the following constraints.
// a.y > b.y = c.y
void FillFlatBottomTriangle(const Triangle *triangle, uint32_t color) 
{
    FillFlatBaseTriangleHelper(&triangle->a, &triangle->b, &triangle->c, color);
}

// Fills a triangle that meets the following constraints.
// c.y < a.y = b.y
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
            float m_ac = static_cast<float>(a->y - c->y) / static_cast<float>(a->x - c->x);
            float b_ac = a->y - (m_ac * a->x);
            x = (b->y - b_ac) / m_ac;
        }

        // This point represents the point where B's y-intercept meets the line AC.
        // The line BD is used to cut the triangle into a flat bottom triangle and a flat top triangle.
        Point d{x, b->y};

        Triangle top{*a, *b, d};
        Triangle bot{d, *b, *c};

        FillFlatBottomTriangle(&top, color);
        FillFlatTopTriangle(&bot, color);
    }
}

// Draws a triangle with respect to the *unscaled* plane.
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
