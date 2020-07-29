#include <cstring>
#include <cstdlib>
#include "Maths.h"
#include "Rasterizer.h"
#include "Renderer.h"

int main()
{
    // TODO: Find a different way to allocate this.
    pixels = (uint32_t **)malloc(PLANE_H * sizeof(uint32_t*));

    for (int i = 0; i < PLANE_H; i++) {
        pixels[i] = (uint32_t *)malloc(PLANE_W * sizeof(uint32_t));
        std::memset(pixels[i], 0, PLANE_W * sizeof(uint32_t));
    }

    // Point p1{20, 800};
    // Point p2{900, 100};
    // DrawLine(p1, p2, 0xFFFF);
    
    // TODO: We need a way to control screen coordinates so that they aren't affected by our FSAA scale.
    Triangle triangle{{1000, 8000}, {10000, 1600}, {11000, 7000}};
    DrawTriangle(&triangle, 0xFF0000, true);
   
    RenderImage();
    return 0;
}
