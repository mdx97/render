#ifndef RASTERIZER_H
#define RASTERIZER_H

#include <cstdint>
#include "Maths.h"

void DrawLine(const Point *p, const Point *q, uint32_t color);
void DrawTriangle(const Triangle *triangle, uint32_t color, bool fill);

#endif
