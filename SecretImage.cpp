#include "SecretImage.h"
#include <iostream>
#include <fstream>


// Constructor: split image into upper and lower triangular arrays
SecretImage::SecretImage(const GrayscaleImage& image) {
    width = image.get_width();
    height = image.get_height();

    upper_triangular = new int[(width * (width + 1)) / 2]; // Upper triangular matrix
    lower_triangular = new int[(width * (width - 1)) / 2]; // Lower triangular matrix

    int upper_counter = 0;
    int lower_counter = 0;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if(j < i){
                lower_triangular[lower_counter++] = image.get_pixel(i,j);
            }
            else{
                upper_triangular[upper_counter++] = image.get_pixel(i,j);
            }
        }
    }
}

// Constructor: instantiate based on data read from file
SecretImage::SecretImage(int w, int h, int * upper, int * lower) {
    width = w;
    height = h;
    upper_triangular = upper;
    lower_triangular = lower;
}


// Destructor: free the arrays
SecretImage::~SecretImage() {
    delete[] upper_triangular;
    delete[] lower_triangular;
}

// Reconstructs and returns the full image from upper and lower triangular matrices.
GrayscaleImage SecretImage::reconstruct() const {
    GrayscaleImage image(width, height);
    int upper_counter = 0;
    int lower_counter = 0;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if(j<i){
                image.set_pixel(i,j,lower_triangular[lower_counter++]);
            }
            else{
                image.set_pixel(i,j,upper_triangular[upper_counter++]);
            }
        }
    }
    return image;
}

// Save the filtered image back to the triangular arrays
void SecretImage::save_back(const GrayscaleImage& image) {

    int upper_counter = 0;
    int lower_counter = 0;

    for (int i = 0; i < image.get_height(); ++i) {
        for (int j = 0; j < image.get_width(); ++j) {
            if(j < i){
                lower_triangular[lower_counter++] = image.get_pixel(i,j);
            }
            else{
                upper_triangular[upper_counter++] = image.get_pixel(i,j);
            }
        }
    }

}

// Save the upper and lower triangular arrays to a file
void SecretImage::save_to_file(const std::string& filename) {
    int upper_size = (width * (width + 1)) / 2;
    int lower_size = (width * (width - 1)) / 2;

    std::ofstream saveFile(filename);

    saveFile << width << " " << height << "\n";

    for (int i = 0; i < upper_size; ++i) {
        saveFile << upper_triangular[i];
        if (i != upper_size - 1) {
            saveFile << " ";
        }
    }
    saveFile << "\n";


    for (int i = 0; i < lower_size; ++i) {
        saveFile << lower_triangular[i];
        if (i != lower_size - 1) {
            saveFile << " ";
        }
    }
    saveFile << "\n";

    saveFile.close();


}

// Static function to load a SecretImage from a file
SecretImage SecretImage::load_from_file(const std::string& filename) {
    std::ifstream loadFile(filename);
    int width, height;

    loadFile >> width >> height;

    int upper_size = (width * (width +1))/2;
    int lower_size = (width * (width -1))/2;

    int* upper_triangular = new int[upper_size];
    int* lower_triangular = new int[lower_size];

    for (int i = 0; i < upper_size; ++i) {
        loadFile >> upper_triangular[i];
    }
    for (int i = 0; i < lower_size; ++i) {
        loadFile >> lower_triangular[i];
    }

    loadFile.close();

    SecretImage secret_image(width,height,upper_triangular,lower_triangular);

    return secret_image;
}

// Returns a pointer to the upper triangular part of the secret image.
int * SecretImage::get_upper_triangular() const {
    return upper_triangular;
}

// Returns a pointer to the lower triangular part of the secret image.
int * SecretImage::get_lower_triangular() const {
    return lower_triangular;
}

// Returns the width of the secret image.
int SecretImage::get_width() const {
    return width;
}

// Returns the height of the secret image.
int SecretImage::get_height() const {
    return height;
}
