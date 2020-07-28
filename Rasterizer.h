#ifndef RASTERIZER_H
#define RASTERIZER_H

#include <cstdint>
#include "Maths.h"

typedef enum color {
    BLACK,
    WHITE,
    RED,
    GREEN,
    BLUE
} Color;

extern unsigned char colors[5][3];

void DrawLine(const Point &p, const Point &q, uint8_t color);
void DrawTriangle(Triangle *triangle, uint8_t color, bool fill);

#endif
