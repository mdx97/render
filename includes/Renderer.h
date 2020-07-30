#ifndef RENDERER_H
#define RENDERER_H

#include <cstdint>

#define SAMPLE_SCALE 16
#define PLANE_W 1920
#define PLANE_H 1080

// typedef enum antialiasing_method_e {
//     FSAA
// } AntiAliasingMethod;

extern uint32_t **pixels;
//extern AntiAliasingMethod antialiasing_method;

void RenderAscii();
void RenderImage();
void InitRenderer();

#endif
