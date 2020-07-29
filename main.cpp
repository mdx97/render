#include <cstring>
#include "Maths.h"
#include "Rasterizer.h"
#include "Renderer.h"

int main()
{
    std::memset(pixels, 0, sizeof(pixels));
    
    for (int i = 0; i < PLANE_H; i++) {
        for (int j = 0; j < PLANE_W; j++) {
            pixel_intensity[i][j] = 1.0f;
        }
    }

    Triangle triangle {{50, 50}, {400, 1000}, {950, 400}};
    DrawTriangle(&triangle, RED, true);
    
    RenderImage();
    return 0;
}
