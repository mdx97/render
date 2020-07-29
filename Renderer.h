#ifndef RENDERER_H
#define RENDERER_H

#include <cstdint>

#define PLANE_W 1024
#define PLANE_H 1024

extern uint8_t pixels[PLANE_H][PLANE_W];
extern float pixel_intensity[PLANE_H][PLANE_W];

void RenderAscii();
void RenderImage();

#endif
