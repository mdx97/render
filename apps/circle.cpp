#include <cmath>
#include "Maths.h"
#include "Rasterizer.h"
#include "Renderer.h"

int main()
{
    InitRenderer();

    Point center{PLANE_W / 2, PLANE_H / 2};
    int radius = 400;
    int arc = 20;

    Point *last = new Point{center.x + radius, center.y};

    for (int i = arc; i <= 360 + arc; i += arc) {
        double rad = d2r(i);
        int x = static_cast<int>(center.x + (radius * std::cos(rad)));
        int y = static_cast<int>(center.y + (radius * std::sin(rad)));
        
        Point *next = new Point{x, y};

        Triangle triangle{center, *next, *last};
        DrawTriangle(&triangle, 0xFFFFFF, true);

        delete last;
        last = next;
    }

    delete last;

    RenderImage();    
    return 0;
}
