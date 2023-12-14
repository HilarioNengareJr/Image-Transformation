
# Compilation

>> g++ -o colortheory main.cpp imageColorUtilities.cpp -lpng

>> ./colortheory

# About

I implement alpha blending of 0.5 to uploaded png files, which I store in an array row_pointers as byte an unsigned 8 bit. 

I demonstrate the blending process by performing the calculation obtained from the alpha blending formula on each pixel assumming there is 3 channels per pixel and color depth of 8.

I save the calculations onto a file called blended_image.txt then read from the file in the save() member function, which thus returns a pointer to an array of type png_bytep.

I use this pointer to write to a png file producing the output image called output.png.