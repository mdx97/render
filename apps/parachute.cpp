#include <cmath>
#include "Maths.h"
#include "Rasterizer.h"
#include "Renderer.h"

int main()
{
    InitRenderer();

    Point center{PLANE_W / 2, PLANE_H / 2};
    int radius = 400;
    int arc = 15;

    uint32_t colors[4] = {0xFF0000, 0xFFFF00, 0x0000FF, 0x00FF00};

    Point *last = new Point{center.x + radius, center.y};

    for (int i = 1; i <= 360 / arc; i++) {
        double rad = d2r(i * arc);
        int x = static_cast<int>(center.x + (radius * std::cos(rad)));
        int y = static_cast<int>(center.y + (radius * std::sin(rad)));
        
        Point *next = new Point{x, y};

        Triangle triangle{center, *next, *last};
        DrawTriangle(&triangle, colors[i % 4], true);

        delete last;
        last = next;
    }

    delete last;

    RenderImage();    
    return 0;
}
