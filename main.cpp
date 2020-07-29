#include <cstring>
#include "Maths.h"
#include "Rasterizer.h"
#include "Renderer.h"

int main()
{
    std::memset(pixels, 0, sizeof(pixels));

    Triangle triangle{{20, 800}, {900, 100}, {800, 900}};
    DrawTriangle(&triangle, 0xFF0000, true);

    RenderImage();
    return 0;
}
