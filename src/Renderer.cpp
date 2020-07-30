#include <cmath>
#include <iostream>
#include "CImg.h"
#include "Rasterizer.h"
#include "Renderer.h"

uint32_t **pixels;

// void RenderAscii()
// {
//     for (int i = 0; i < PLANE_H; i++) {
//         for (int j = 0; j < PLANE_W; j++) {
//             if (pixels[i][j] != 0) {
//                 std::cout << pixels[i][j] << " ";
//             } else {
//                 std::cout << "  ";
//             }
//         }
//         std::cout << std::endl;
//     }
// }

// Converts a hexidecimal color code into RGB format.
// note: the hex data is stored in the least significant bits of the 32 bit number.
// example: 0x[00]FFFFFF = (255, 255, 255)
void HexToRgb(uint32_t hex, unsigned char *rgb)
{
    rgb[0] = (hex >> 16) & 0xFF;
    rgb[1] = (hex >> 8) & 0xFF;
    rgb[2] = hex & 0xFF;
}

// Renders the image currently drawn in the front buffer (pixels).
void RenderImage()
{
    cimg_library::CImg<unsigned char> image(PLANE_W, PLANE_H, 1, 3);
    int sampled = std::pow(SAMPLE_SCALE, 2);

    for (int i = 0; i < PLANE_H * SAMPLE_SCALE; i += SAMPLE_SCALE) {
        for (int j = 0; j < PLANE_W * SAMPLE_SCALE; j += SAMPLE_SCALE) {
            int rsum = 0, gsum = 0, bsum = 0;
            unsigned char color[3] = { 0, 0, 0 };

            for (int k = 0; k < SAMPLE_SCALE; k++) {
                for (int l = 0; l < SAMPLE_SCALE; l++) {
                    int row = i < PLANE_H - SAMPLE_SCALE ? i + k : i - k;
                    int col = j < PLANE_W - SAMPLE_SCALE ? j + l : j - l;

                    HexToRgb(pixels[row][col], color);
                    rsum += color[0];
                    gsum += color[1];
                    bsum += color[2];
                }
            }

            color[0] = rsum / sampled;
            color[1] = gsum / sampled;
            color[2] = bsum / sampled;

            int x = j / SAMPLE_SCALE;
            int y = i / SAMPLE_SCALE;

            image.draw_point(x, y, color);
        }
    }

    cimg_library::CImgDisplay display(PLANE_W, PLANE_H, "Render", 3, false, false);
    image.display(display);

    while(!display.is_closed());

    // // Keeping this commented out because sometimes we need to be able to zoom in on pixels.
    // image.display("Render");
    // while(true); // @FIXME: May need to set up a seperate display for the debug code, but it's debug code \_O_/.

}

// Allocates all memory the renderer needs.
void InitRenderer()
{
    // @FIXME: Find a different way to allocate this.
    pixels = (uint32_t **)malloc(PLANE_H * SAMPLE_SCALE * sizeof(uint32_t*));

    for (int i = 0; i < PLANE_H * SAMPLE_SCALE; i++) {
        pixels[i] = (uint32_t *)malloc(PLANE_W * SAMPLE_SCALE * sizeof(uint32_t));
        std::memset(pixels[i], 0, PLANE_W * SAMPLE_SCALE * sizeof(uint32_t));
    }
}
