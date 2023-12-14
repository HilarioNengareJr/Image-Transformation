#ifndef IMAGE_COLOR_UTILITIES_H
#define IMAGE_COLOR_UTILITIES_H

#include <string>
#include <png.h>

namespace ICU
{
    typedef png_bytep BYTEP;

    class ImageType
    {
    private:
        std::size_t width;
        std::size_t height;

    public:
        BYTEP *load(const std::string &image_path);
        void blend(BYTEP *forecolor, BYTEP *backcolor, int fore_width, int fore_height, double alpha);
        BYTEP *save(const std::string &blended_image_path, int fore_width, int fore_height);
        void savePNG(const std::string &output_path, BYTEP *row_pointers);
        // Accessors
        std::size_t getWidth() const { return this->width; }
        std::size_t getHeight() const { return this->height; }
    };
}

#endif
