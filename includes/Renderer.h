#ifndef RENDERER_H
#define RENDERER_H

#include <cstdint>

#define SAMPLE_SCALE 8
#define PLANE_W 15360
#define PLANE_H 8640

// #define SAMPLE_SCALE 2
// #define PLANE_W 128
// #define PLANE_H 128

extern uint32_t **pixels;

void RenderAscii();
void RenderImage();
void InitRenderer();

#endif
