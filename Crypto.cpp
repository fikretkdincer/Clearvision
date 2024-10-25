#include "Crypto.h"
#include "GrayscaleImage.h"
#include <cmath>


// Extract the least significant bits (LSBs) from SecretImage, calculating x, y based on message length
std::vector<int> Crypto::extract_LSBits(SecretImage& secret_image, int message_length) {
    std::vector<int> LSB_array;

    GrayscaleImage image = secret_image.reconstruct();

    int width = image.get_width();
    int height = image.get_height();
    int required_bits = message_length * 7;

    if(width * height < required_bits){
        throw std::runtime_error("Invalid Size");
    }

    int starting_position = width*height - required_bits;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if(i * width + j >= starting_position){
                LSB_array.push_back(image.get_pixel(i,j)%2);
            }
        }
    }
    return LSB_array;
}


// Decrypt message by converting LSB array into ASCII characters
std::string Crypto::decrypt_message(const std::vector<int>& LSB_array) {
    std::string message;
    if(LSB_array.size() % 7 != 0){
        throw std::runtime_error("Invalid Size");
    }

    for (int i = 0; i < LSB_array.size(); i+=7) {
        int ascii = 0;
        int power = 6;
        for (int j = 0; j < 7; ++j) {
            ascii += LSB_array[i+j] % 2 * pow(2,power);
            power--;
        }
        message += (char)ascii;
    }

    // TODO: Your code goes here.
    return message;
}

// Encrypt message by converting ASCII characters into LSBs
std::vector<int> Crypto::encrypt_message(const std::string& message) {
    std::vector<int> LSB_array;

    for(char c: message){
        int ascii = static_cast<unsigned char>(c);
        int bits[7];

        for (int i = 0; i < 7; ++i) {
            bits[i] = ascii %2;
            ascii /= 2;
        }
        for (int i = 6; i >= 0; --i) {
            LSB_array.push_back(bits[i]);
        }
    }
    return LSB_array;
}

// Embed LSB array into GrayscaleImage starting from the last bit of the image
SecretImage Crypto::embed_LSBits(GrayscaleImage& image, const std::vector<int>& LSB_array) {
    
    int width = image.get_width();
    int height = image.get_height();
    
    if(width * height < LSB_array.size()){
        throw std::runtime_error("Invalid Size");
    }
    
    int starting_position = width * height - LSB_array.size();
    int iterator = 0;


    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (i * width + j >= starting_position) {
                int currentPixel = image.get_pixel(i, j);
                int value = LSB_array[iterator];

                if (currentPixel % 2 != value) {
                    if (value == 1) {
                        image.set_pixel(i, j, currentPixel + 1);
                    } else {
                        image.set_pixel(i, j, currentPixel - 1);
                    }
                }

                iterator++;
            }
        }
    }

    SecretImage secret_image(image);
    return secret_image;
}
