#include "Filter.h"
#include <algorithm>
#include <cmath>
#include <vector>


// Mean Filter
void Filter::apply_mean_filter(GrayscaleImage& image, int kernelSize) {

    GrayscaleImage copy(image);

    for (int i = 0; i < copy.get_height(); ++i) {
        for (int j = 0; j < copy.get_width(); ++j) {
            int new_pixel = Filter::calculate_neighbor(copy, kernelSize,i,j);
            image.set_pixel(i,j,new_pixel);
        }
    }
}

// Gaussian Smoothing Filter
void Filter::apply_gaussian_smoothing(GrayscaleImage& image, int kernelSize, double sigma) {
    GrayscaleImage copy(image);

    for (int i = 0; i < copy.get_height(); ++i) {
        for (int j = 0; j < copy.get_width(); ++j) {
            int new_pixel = Filter::calculate_gaussian(copy,kernelSize,i,j,sigma);
            image.set_pixel(i,j, new_pixel);
        }
    }
}

// Unsharp Masking Filter
void Filter::apply_unsharp_mask(GrayscaleImage& image, int kernelSize, double amount) {
    GrayscaleImage blurred(image);

    Filter::apply_gaussian_smoothing(blurred, kernelSize, 1.0);

    for (int i = 0; i < image.get_height(); ++i) {
        for (int j = 0; j < image.get_width(); ++j) {
            int original_pixel = image.get_pixel(i, j);
            int blurred_pixel = blurred.get_pixel(i, j);

            double unsharp_value = original_pixel + amount * (original_pixel - blurred_pixel);

            int final_pixel = std::min(std::max(static_cast<int>(unsharp_value), 0), 255);

            image.set_pixel(i, j, final_pixel);
        }
    }
}
// Calculate neighboring pixels mean value
int Filter::calculate_neighbor(GrayscaleImage& image, int kernelSize, int pos_x, int pos_y){
    // Calculating mean values of the neighboring pixels here, aiming to have a cleaner code.
    int total = 0;
    int borderSize = kernelSize / 2;
    for (int i = pos_x - borderSize; i <= pos_x + borderSize; ++i) {
        for (int j = pos_y - borderSize; j <= pos_y + borderSize; ++j) {
            if(i >= 0 && j >= 0){
                if(i < image.get_height() && j < image.get_width()){
                    total += image.get_pixel(i,j);
                }
            }
        }
    }
    return total / (kernelSize * kernelSize);
}

int Filter::calculate_gaussian(GrayscaleImage& image, int kernelSize, int pos_x, int pos_y, double sigma){
    double total = 0;
    double normalizer = 0;
    double gaussian;
    int borderSize = kernelSize / 2;
    for (int i = pos_x - borderSize; i <= pos_x + borderSize; ++i) {
        for (int j = pos_y - borderSize; j <= pos_y + borderSize; ++j) {
            gaussian = Filter::calculate_gaussian_kernel(abs(i-pos_x), abs(j-pos_y),sigma);
            normalizer += gaussian;
            if(i >= 0 && j >= 0){
                if(i < image.get_height() && j < image.get_width()){
                    total += image.get_pixel(i,j)*gaussian;
                }
            }
        }
    }
    total /= normalizer;
    return (int)total;
}

double Filter::calculate_gaussian_kernel(int dis_x, int dis_y, double sigma){
     const double pi = 3.14159265358979311599796346854;
     double exponent = -(dis_x * dis_x + dis_y * dis_y) / (2 * sigma * sigma);
     double result = (1 / (2 * pi * sigma * sigma)) * exp(exponent);

     return result;
}


