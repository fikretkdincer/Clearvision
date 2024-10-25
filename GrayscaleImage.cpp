#include "GrayscaleImage.h"
#include <iostream>
#include <cstring>  // For memcpy
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <stdexcept>


// Constructor: load from a file
GrayscaleImage::GrayscaleImage(const char* filename) {

    // Image loading code using stbi
    int channels;
    unsigned char* image = stbi_load(filename, &width, &height, &channels, STBI_grey);

    if (image == nullptr) {
        std::cerr << "Error: Could not load image " << filename << std::endl;
        exit(1);
    }

    data = new int *[height];
    for(int i = 0; i< height; i++){
        data[i] = new int[width];
    }

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            data[i][j] = static_cast<int>(image[i * width + j]);
        }
    }

    // Free the dynamically allocated memory of stbi image
    stbi_image_free(image);
}

// Constructor: initialize from a pre-existing data matrix
GrayscaleImage::GrayscaleImage(int** inputData, int h, int w): width(w), height(h) {
    data = new int*[h];
    for(int i = 0; i < h; i++){
        data[i] = new int[w];
    }

    for(int i = 0; i < h; i++){
        for(int j = 0; j< w; j++){
            data[i][j] = inputData[i][j];
        }
    }
}

// Constructor to create a blank image of given width and height
GrayscaleImage::GrayscaleImage(int w, int h) : width(w), height(h) {
    data = new int*[h];
    for(int i = 0; i < h; i++){
        data[i] = new int[w];
    }
}

// Copy constructor
GrayscaleImage::GrayscaleImage(const GrayscaleImage& other) {
    width = other.get_width();
    height = other.get_height();

    data = new int* [other.get_height()];
    for(int i = 0; i < other.get_height(); i++){
        data[i] = new int[width];
    }

    for(int i = 0; i<other.get_height(); i++){
        for(int j = 0; j<other.get_width(); j++){
            data[i][j] = other.get_pixel(i,j);
        }
    }
}

// Destructor
GrayscaleImage::~GrayscaleImage() {
    for(int i = 0; i < height; i++){

        delete[] data[i];
    }
    delete[] data;
}

// Equality operator
bool GrayscaleImage::operator==(const GrayscaleImage& other) const {
    if (width != other.get_width() || height != other.get_height()) {
        return false;
    }

    for(int i = 0; i < other.get_height(); i++){
        for(int j = 0; j < other.get_width(); j++){
            if(data[i][j] != other.get_pixel(i,j)){
                return false;
            }
        }
    }
    return true;
}

// Addition operator
GrayscaleImage GrayscaleImage::operator+(const GrayscaleImage& other) const {
    if (width != other.get_width() || height != other.get_height()) {
        throw std::invalid_argument("Images must have the same dimensions for subtraction.");
    }
    GrayscaleImage result(width, height);

    for(int i = 0; i<height;i++){
        for(int j = 0; j<width; j++){
            int adder = data[i][j] + other.get_pixel(i,j);
            if(adder > 255){
                result.data[i][j] = 255;
            }else{
                result.data[i][j] = adder;
            }
        }
    }
    return result;
}

// Subtraction operator
GrayscaleImage GrayscaleImage::operator-(const GrayscaleImage& other) const {
    // Create a new image for the result
    if (width != other.get_width() || height != other.get_height()) {
        throw std::invalid_argument("Images must have the same dimensions for subtraction.");
    }
    GrayscaleImage result(width, height);

    for(int i = 0; i<height;i++){
        for(int j = 0; j<width; j++){
            int subtracter = data[i][j] - other.get_pixel(i,j);
            if(subtracter < 0){
                result.data[i][j] = 0;
            }else{
                result.data[i][j] = subtracter;
            }
        }
    }
    return result;
}

// Get a specific pixel value
int GrayscaleImage::get_pixel(int row, int col) const {
    return data[row][col];
}

// Set a specific pixel value
void GrayscaleImage::set_pixel(int row, int col, int value) {
    data[row][col] = value;
}

// Function to save the image to a PNG file
void GrayscaleImage::save_to_file(const char* filename) const {
    // Create a buffer to hold the image data in the format stb_image_write expects
    unsigned char* imageBuffer = new unsigned char[width * height];

    // Fill the buffer with pixel data (convert int to unsigned char)
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            imageBuffer[i * width + j] = static_cast<unsigned char>(data[i][j]);
        }
    }

    // Write the buffer to a PNG file
    if (!stbi_write_png(filename, width, height, 1, imageBuffer, width)) {
        std::cerr << "Error: Could not save image to file " << filename << std::endl;
    }

    // Clean up the allocated buffer
    delete[] imageBuffer;
}
