
#include <iostream>
#include "Image.hpp"
#include <cmath>
#include <string>

namespace ImageSpace
{
    class ImageEditing : public ColorImage
    {
    public:
        void rotate(float theta, const std::size_t w, const std::size_t h);
        void shear(float thresh_x, float thresh_y, const std::size_t w, const std::size_t h);
        void reflect(const std::size_t w, const std::size_t h);
        void scale(float thresh_x, float thresh_y, const std::size_t w, const std::size_t h);
        void translate(float thresh_x, float thresh_y, const std::size_t w, const std::size_t h);
    };

    // Rotation member function
    void ImageEditing::rotate(float theta, const std::size_t w, const std::size_t h)
    {
        float cos_theta = cos(theta);
        float sin_theta = sin(theta);
        int new_x, new_y;

        // Instance to save the rotated image
        ColorImage rotated_image(w, h);

        for (int y = 0; y < GetHeight(); y++)
        {
            for (int x = 0; x < GetWidth(); x++)
            {
                float id_matrix[3] = {static_cast<float>(x), static_cast<float>(y), 1.0f};
                float x_prime, y_prime, z;

                x_prime = cos_theta * id_matrix[0] - sin_theta * id_matrix[1];
                y_prime = sin_theta * id_matrix[0] + cos_theta * id_matrix[1];
                z = id_matrix[2];

                new_x = static_cast<int>(x_prime);
                new_y = static_cast<int>(y_prime);

                // Restrain coordinates to avoid boundary overlapping
                if (new_y > -1 && new_y < h && new_x > -1 && new_x < w)
                {
                    // Copying the pixel value from the original image to the rotated image
                    rotated_image(new_x, new_y) = (*this)(x, y);
                }
            }
        }

        // Saving the rotated image
        rotated_image.Save("rotated_image.png");
    }

    // Shear member function
    void ImageEditing::shear(float thresh_x, float thresh_y, const std::size_t w, const std::size_t h)
    {
        float shear_matrix[3][3] = {
            {1, thresh_x, 0},
            {thresh_y, 1, 0},
            {0, 0, 1}};

        // Instance to store sheared image
        ColorImage sheared_image(w, h);

        for (int y = 0; y < h; y++)
        {
            for (int x = 0; x < w; x++)
            {
                float id_matrix[3] = {static_cast<float>(x), static_cast<float>(y), 1.0f};
                int new_x, new_y;
                int z;

                // Performing shear
                new_x = shear_matrix[0][0] * id_matrix[0] + shear_matrix[0][1] * id_matrix[1] + shear_matrix[0][2] * id_matrix[2];
                new_y = shear_matrix[1][0] * id_matrix[0] + shear_matrix[1][1] * id_matrix[1] + shear_matrix[1][2] * id_matrix[2];
                z = shear_matrix[2][0] * id_matrix[0] + shear_matrix[2][1] * id_matrix[1] + shear_matrix[2][2] * id_matrix[2];

                // Normalizing the primes
                new_x = static_cast<int>(new_x / z);
                new_y = static_cast<int>(new_y / z);

                if (new_y > -1 && new_y < h && new_x > -1 && new_x < w)
                {
                    sheared_image(new_x, new_y) = (*this)(x, y);
                }
            }
        }

        // Saving the sheared image
        sheared_image.Save("sheared_image.png");
    }

    // Translation member function
    void ImageEditing::translate(float thresh_x, float thresh_y, const std::size_t w, const std::size_t h)
    {
        float translation_matrix[3][3] = {
            {1, 0, thresh_x},
            {0, 1, thresh_y},
            {0, 0, 1}};

        ColorImage translated_image(w, h);

        for (int y = 0; y < h; y++)
        {
            for (int x = 0; x < w; x++)
            {
                float id_matrix[3] = {static_cast<float>(x), static_cast<float>(y), 1.0f};
                float x_prime, y_prime, z;

                x_prime = translation_matrix[0][0] * id_matrix[0] + translation_matrix[0][1] * id_matrix[1] + translation_matrix[0][2] * id_matrix[2];
                y_prime = translation_matrix[1][0] * id_matrix[0] + translation_matrix[1][1] * id_matrix[1] + translation_matrix[1][2] * id_matrix[2];
                z = translation_matrix[2][0] * id_matrix[0] + translation_matrix[2][1] * id_matrix[1] + translation_matrix[2][2] * id_matrix[2];

                int new_x = static_cast<int>(x_prime / z);
                int new_y = static_cast<int>(y_prime / z);

                if (new_y > -1 && new_y < h && new_x > -1 && new_x < w)
                {

                    translated_image(new_x, new_y) = (*this)(x, y);
                }
            }
        }
        translated_image.Save("translated_image.png");
    }

    // Scaling member function
    void ImageEditing::scale(float thresh_x, float thresh_y, const std::size_t w, const std::size_t h)
    {
        // Scaling matrix
        float scale_matrix[3][3] = {
            {thresh_x, 0, 0},
            {0, thresh_y, 0},
            {0, 0, 1}};

        ColorImage scaled_image(w, h);

        for (int y = 0; y < h; y++)
        {
            for (int x = 0; x < w; x++)
            {
                float id_matrix[3] = {static_cast<float>(x), static_cast<float>(y), 1.0f};
                float x_prime, y_prime, z;

                x_prime = scale_matrix[0][0] * id_matrix[0] + scale_matrix[0][1] * id_matrix[1] + scale_matrix[0][2] * id_matrix[2];
                y_prime = scale_matrix[1][0] * id_matrix[0] + scale_matrix[1][1] * id_matrix[1] + scale_matrix[1][2] * id_matrix[2];
                z = scale_matrix[2][0] * id_matrix[0] + scale_matrix[2][1] * id_matrix[1] + scale_matrix[2][2] * id_matrix[2];

                // Normalizing the primes
                int new_x = static_cast<int>(x_prime / z);
                int new_y = static_cast<int>(y_prime / z);

                if (new_y > -1 && new_y < h && new_x > -1 && new_x < w)
                {

                    scaled_image(new_x, new_y) = (*this)(x, y);
                }
            }
        }

        scaled_image.Save("scaled_image.png");
    }

    // X-axis reflection member function
    void ImageEditing::reflect(const std::size_t w, const std::size_t h)
    {
        float reflection_matrix[3][3] = {
            {1, 0, 0},
            {0, -1, 0},
            {0, 0, 1}};

        ColorImage reflected_image(w, h);

        for (int y = 0; y < h; y++)
        {
            for (int x = 0; x < w; x++)
            {
                float id_matrix[3] = {static_cast<float>(x), static_cast<float>(y), 1.0f};
                int new_x, new_y, z;

                new_x = reflection_matrix[0][0] * id_matrix[0] + reflection_matrix[0][1] * id_matrix[1] + reflection_matrix[0][2] * id_matrix[2];
                new_y = reflection_matrix[1][0] * id_matrix[0] + reflection_matrix[1][1] * id_matrix[1] + reflection_matrix[1][2] * id_matrix[2];
                z = reflection_matrix[2][0] * id_matrix[0] + reflection_matrix[2][1] * id_matrix[1] + reflection_matrix[2][2] * id_matrix[2];

                // Avoid segmentation fault
                new_x = std::clamp(static_cast<int>(new_x), 0, static_cast<int>(w) - 1);
                new_y = std::clamp(static_cast<int>(new_y), 0, static_cast<int>(h) - 1);

                reflected_image(x, y) = (*this)(new_x, new_y);
            }
        }
        reflected_image.Save("reflected_image.png");
    }

}

int main()
{
    ImageSpace::ImageEditing img;

    img.Load("./arrow.png");
    img.rotate(0.5, img.GetWidth() * 2, img.GetHeight() * 2);
    img.shear(0.2, 0.3, img.GetWidth(), img.GetHeight());
    img.scale(2.0, 2.0, img.GetWidth(), img.GetHeight());
    img.translate(30.0, 20.0, img.GetWidth(), img.GetHeight());
    img.reflect(img.GetWidth(), img.GetHeight());
    return 0;
}
