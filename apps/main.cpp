#include <cstring>
#include <cstdlib>
#include "Maths.h"
#include "Rasterizer.h"
#include "Renderer.h"

int main()
{
    InitRenderer();
    
    // Point p1{20, 800};
    // Point p2{900, 100};
    // DrawLine(p1, p2, 0xFFFF);
    
    // TODO: We need a way to control screen coordinates so that they aren't affected by our FSAA scale.
    Triangle triangle{{1000, 2000}, {1000, 7000}, {5000, 2000}};
    DrawTriangle(&triangle, 0xFF0000, true);
   
    RenderImage();
    return 0;
}
