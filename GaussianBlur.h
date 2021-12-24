//
// Gaussian Blur filter, with a set size of 5 x 5
//

#ifndef PROJECT2_GAUSSIANBLUR_H
#define PROJECT2_GAUSSIANBLUR_H

#include <numeric>
#include <vector>
#include "image.h"
#include <cmath>
#include "ImageFilter.h"
#include <map>


// Gaussian Blur: kernel of 5x5
class GaussianBlur: public ImageFilter {

public:

    // constructor to initialize a basic kernel of 3x3
    GaussianBlur() : sigma(1.0){};

    void setSigma(double s){
        sigma = s;
    }

    // Update the image filter and compute the output
    virtual void update()
    {
        // Didn't touch _input, cast it back to const; then execute()
        execute(getInput());
    }

protected:

    void generateKernel(double GaussianKernel[5][5]);


    // execute function with kernel
    virtual void execute(const Image& i);

private:
    // private kernel
    double kernel[5][5];
    double sigma;
};

void GaussianBlur::generateKernel(double (*GaussianKernel)[5]){

    double r;
    double s = 2*sigma*sigma;
    double sum = 0;
    const double pi = 3.1415; //be more specific

    // Generate the kernel
    for(int x = -2; x <3; x++){
        for(int y = -2; y <3; y++){
            r = pow(x,2)+pow(y,2);
            GaussianKernel[x+2][y+2] = exp(-(r/s))/(pi*s); // Formula from wikipedia https://en.wikipedia.org/wiki/Gaussian_blur
            sum = sum + GaussianKernel[x+2][y+2];
        }
    }
    // Normalise kernel
    for(int i = 0; i <5; i++){
        for(int j = 0; j <5; j++){
            GaussianKernel[i][j] = GaussianKernel[i][j]/sum;
        }
    }
}

void GaussianBlur::execute(const Image &i) {
    _output.resize(i.size()); // maybe change this to copy constructor to directly manipulate pixel values

    // Get the kernel
    generateKernel(kernel);

    for (unsigned int t{0}; t < i.dim(4); ++t) {
        for (unsigned int c{0}; c < i.dim(3); ++c) {
            for (unsigned int z{0}; z < i.dim(2); ++z) {
                for (unsigned int y{0}; y < i.dim(1); ++y) {
                    for (unsigned int x{0}; x < i.dim(0); ++x) {

                        int sum = 0;

                        for (int kernel_x = -2; kernel_x < 3; ++kernel_x) {
                            for (int kernel_y = -2; kernel_y < 3; ++kernel_y) {

                                sum = sum + kernel[kernel_x+2][kernel_y+2] * i(x+kernel_x, y+kernel_y, z, c,t);
                            }
                        }
                        // conv_output returns a reference/address, we provide sum
                        _output(x,y,z,t,c) = sum;
                    }
                }
            }
        }
    }
}


#endif //PROJECT2_GAUSSIANBLUR_H
