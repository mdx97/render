#include <cstring>
#include "Maths.h"
#include "Rasterizer.h"
#include "Renderer.h"

int main()
{
    std::memset(pixels, 0, sizeof(pixels));

    Triangle triangle {{50, 600}, {800, 1000}, {950, 50}};
    DrawTriangle(&triangle, RED, true);
    
    RenderImage();
    return 0;
}
