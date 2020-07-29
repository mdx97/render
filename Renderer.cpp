#include <iostream>
#include "CImg.h"
#include "Rasterizer.h"
#include "Renderer.h"

uint8_t pixels[PLANE_H][PLANE_W];
float pixel_intensity[PLANE_H][PLANE_W];

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
            unsigned char color[3];
            std::memcpy(color, colors[pixels[i][j]], 3);

            color[0] = color[0] * pixel_intensity[i][j];
            color[1] = color[1] * pixel_intensity[i][j];
            color[2] = color[2] * pixel_intensity[i][j];

            image.draw_point(j, i, color);
        }
    }

    image.display("Render");
}
