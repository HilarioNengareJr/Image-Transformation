#include "imageColorUtilities.hpp"
#include <iostream>
#include <fstream>
#include <png.h>
#include <string>
#include <cstddef>

namespace ICU
{
    // For code readability
    typedef png_byte BYTE;
    typedef png_bytep BYTEP;

    BYTEP *ImageType::load(const std::string &image_path)
    {
        // Opening a png file for read
        FILE *file = fopen(image_path.c_str(), "rb");
        if (!file)
        {
            std::cerr << "Error occured during file read." << std::endl;
            return nullptr;
        }

        // Creating pointer to png struct
        png_structp png_pointer = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_pointer)
        {
            std::cerr << "Error: could not implement pointer to struct for the file." << std::endl;
            fclose(file);
            return nullptr;
        }

        // Creating pointer to png information struct
        png_infop info_pointer = png_create_info_struct(png_pointer);
        if (!info_pointer)
        {
            std::cerr << "Error: could not implement pointer to struct for the file information." << std::endl;
            // Cleaning up and closing the file if any errors arise
            png_destroy_read_struct(&png_pointer, nullptr, nullptr);
            fclose(file);
            return nullptr;
        }

        // Loading I/O with the png file
        png_init_io(png_pointer, file);
        png_read_info(png_pointer, info_pointer);

        // Retrieving width and height of the image
        this->width = png_get_image_width(png_pointer, info_pointer);
        this->height = png_get_image_height(png_pointer, info_pointer);
        BYTE color_type = png_get_color_type(png_pointer, info_pointer);
        BYTE bit_depth = png_get_bit_depth(png_pointer, info_pointer);

        // Memory allocation for the image data
        BYTEP *row_pointers = new BYTEP[height];
        for (int i = 0; i < height; i++)
        {
            row_pointers[i] = new BYTE[width * 3]; // 3 bytes per pixel
        }

        // Performing a read on the image data
        png_read_image(png_pointer, row_pointers);

        return row_pointers;
    }

    void ImageType::blend(BYTEP *forecolor, BYTEP *backcolor, int fore_width, int fore_height, double alpha)
    {
        std::ofstream output_file("blended_image.txt");

        // Writing the image data to a .txt file
        if (output_file.is_open())
        {
            // Applying alpha blend to the pixels
            for (int j = 0; j < fore_height; j++)
            {
                for (int i = 0; i < fore_width * 3; i += 3)
                {
                    output_file << int(backcolor[j][i]) * alpha + int(forecolor[j][i]) * (1 - alpha)
                                << int(backcolor[j][i + 1]) * alpha + int(forecolor[j][i + 1]) * (1 - alpha)
                                << int(backcolor[j][i + 2]) * alpha + int(forecolor[j][i + 2]) * (1 - alpha) << " ";
                }

                std::cout << std::endl;
            }
        }
        else
        {
            std::cerr << "Error occurred pertaining to file." << std::endl;
        }
    }

    BYTEP *ImageType::save(const std::string &blended_image_path, int fore_width, int fore_height)
    {
        std::ifstream input_file("./blended_image.txt");
        if (!input_file.is_open())
        {
            std::cerr << "Error opening text file for read." << std::endl;
            return nullptr;
        }

        BYTEP *row_pointers = new BYTEP[fore_height];
        for (int i = 0; i < fore_height; i++)
        {
            row_pointers[i] = new BYTE[fore_width * 3]; // 3 bytes per pixel
            for (int j = 0; j < fore_width * 3; j++)
            {
                int pixelValue;
                input_file >> pixelValue;
                // Casting back to byte from int
                row_pointers[i][j] = static_cast<BYTE>(pixelValue);
            }
        }

        input_file.close();
        return row_pointers;
    }

    void ImageType::savePNG(const std::string &output_path, BYTEP *rowpointers)
    {
        // Opening the output file for write
        FILE *file = fopen(output_path.c_str(), "wb");
        if (!file)
        {
            std::cerr << "Error opening output file for write." << std::endl;
            return;
        }

        png_structp png_pointer = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_pointer)
        {
            std::cerr << "Error creating png struct." << std::endl;
            fclose(file);
            return;
        }

        png_infop info_ptr = png_create_info_struct(png_pointer);
        if (!info_ptr)
        {
            std::cerr << "Error creating png info struct." << std::endl;
            png_destroy_write_struct(&png_pointer, NULL);
            fclose(file);
            return;
        }

        png_init_io(png_pointer, file);

        // Setting image information
        png_set_IHDR(png_pointer, info_ptr, this->width, this->height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

        // Writing image data
        BYTEP *row_pointers = new BYTEP[this->height];
        for (int i = 0; i < this->height; i++)
        {
            row_pointers[i] = rowpointers[i];
        }
        png_set_rows(png_pointer, info_ptr, row_pointers);
        png_write_png(png_pointer, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

        png_destroy_write_struct(&png_pointer, &info_ptr);
        fclose(file);
    }

}