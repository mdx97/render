#ifndef RENDERER_H
#define RENDERER_H

#include <cstdint>

#define SAMPLE_SCALE 8
#define PLANE_W 1024
#define PLANE_H 1024

extern uint32_t pixels[PLANE_H][PLANE_W];

void RenderAscii();
void RenderImage();

#endif
