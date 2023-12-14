#include <iostream>
#include <png.h>
#include "imageColorUtilities.hpp"

int main()
{

    ICU::ImageType png;
    std::size_t height = png.getHeight();
    std::size_t width = png.getWidth();

    png.blend(png.load("./blue.png"), png.load("./red.png"), width, height, 0.2);
    png.savePNG("./output.png", png.save("./blended_image.txt", 1000, 1000));

    return 0;
}