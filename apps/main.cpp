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
    
    Triangle triangle{{50, 500}, {50, 1000}, {1000, 50}};
    DrawTriangle(&triangle, 0xFF0000, true);
   
    RenderImage();
    return 0;
}
