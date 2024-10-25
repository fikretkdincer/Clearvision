#ifndef FILTER_H
#define FILTER_H

#include "GrayscaleImage.h"

class Filter {
public:
    // Apply the Mean Filter
    static void apply_mean_filter(GrayscaleImage& image, int kernelSize = 3);

    // Apply Gaussian Smoothing Filter
    static void apply_gaussian_smoothing(GrayscaleImage& image, int kernelSize = 3, double sigma = 1.0);

    // Apply Unsharp Masking Filter
    static void apply_unsharp_mask(GrayscaleImage& image, int kernelSize = 3, double amount = 1.5);

    static int calculate_neighbor(GrayscaleImage& image, int kernelSize, int pos_x, int pos_y);

    static double calculate_gaussian_kernel(int dis_x, int dis_y, double sigma);

    static int calculate_gaussian(GrayscaleImage& image, int kernelSize, int pos_x, int pos_y, double sigma);

};

#endif // FILTER_H
