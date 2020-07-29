#include <cmath>
#include <iostream>
#include "CImg.h"
#include "Rasterizer.h"
#include "Renderer.h"

uint32_t **pixels;

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

void HexToRgb(uint32_t hex, unsigned char *rgb)
{
    rgb[0] = (hex >> 16) & 0xFF;
    rgb[1] = (hex >> 8) & 0xFF;
    rgb[2] = hex & 0xFF;
}

void RenderImage()
{
    cimg_library::CImg<unsigned char> image(PLANE_W / SAMPLE_SCALE, PLANE_H / SAMPLE_SCALE, 1, 3);

    for (int i = 0; i < PLANE_H; i += SAMPLE_SCALE) {
        for (int j = 0; j < PLANE_W; j += SAMPLE_SCALE) {
            int rsum = 0, gsum = 0, bsum = 0;
            int sampled = 0;
            unsigned char color[3] = { 0, 0, 0 };

            for (int k = 0; k < SAMPLE_SCALE; k++) {
                for (int l = 0; l < SAMPLE_SCALE; l++) {
                    // TODO: Due to this logic, we just simply won't sample the bottom and right N pixels, where N = SAMPLE_SCALE.
                    // Need to play around with ideas of reversing the sampling direction (try up and left instead of down and right for this part of the image maybe?)
                    if (i + k < PLANE_H && j + l < PLANE_W) {
                        HexToRgb(pixels[i + k][j + l], color);
                        rsum += color[0];
                        gsum += color[1];
                        bsum += color[2];
                        sampled++;
                    }
                }
            }

            color[0] = rsum / sampled;
            color[1] = gsum / sampled;
            color[2] = bsum / sampled;

            image.draw_point(j / SAMPLE_SCALE, i / SAMPLE_SCALE, color);
        }
    }

    cimg_library::CImgDisplay display(PLANE_W / SAMPLE_SCALE, PLANE_H / SAMPLE_SCALE, "Render", 3, false, false);
    image.display(display);

    // Keeping this commented out because sometimes we need to be able to zoom in on pixels.
    // image.display("Render");

    // TODO: Eventually we're going to have to find a different way to wait, as we'll want to be able to call RenderImage multiple times.
    while(true);
}

void InitRenderer()
{
    // TODO: Find a different way to allocate this.
    pixels = (uint32_t **)malloc(PLANE_H * sizeof(uint32_t*));

    for (int i = 0; i < PLANE_H; i++) {
        pixels[i] = (uint32_t *)malloc(PLANE_W * sizeof(uint32_t));
        std::memset(pixels[i], 0, PLANE_W * sizeof(uint32_t));
    }
}
