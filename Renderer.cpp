#include <iostream>
#include "CImg.h"
#include "Rasterizer.h"
#include "Renderer.h"

uint8_t pixels[PLANE_H][PLANE_W];

void RenderAscii()
{
    for (int i = 0; i < PLANE_H; i++) {
        for (int j = 0; j < PLANE_W; j++) {
            if (pixels[i][j] != 0) {
                std::cout << pixels[i][j] << " ";
            } else {
                std::cout << "  ";
            }
        }
        std::cout << std::endl;
    }
}

void RenderImage()
{
    cimg_library::CImg<unsigned char> image(PLANE_W, PLANE_H, 1, 3);

    for (int i = 0; i < PLANE_H; i++) {
        for (int j = 0; j < PLANE_W; j++) {
            image.draw_point(j, i, colors[pixels[i][j]]);
        }
    }

    image.display("Render");
}
