//
// The Sobel filter, with two set kernels for Gx and Gy.
//

#ifndef PROJECT2_SOBELFILTER_H
#define PROJECT2_SOBELFILTER_H

#include <numeric>
#include <vector>
#include "image.h"
#include <cmath>
#include "ImageFilter.h"

// todo: finish this

// Sobelfilter. Inherits from convolutionalfilters.
class Sobel: public ImageFilter {

public:

    // Constructor to initialize a basic kernel of 3x3
    Sobel() : kernelX{1,0,-1,2,0,-2,1,0,-1}, kernelY{1,2,1,0,0,0,-1,-2,-1}{}

    // Update the image filter and compute the output
    virtual void update()
    {
        execute(getInput());
    }

protected:

    // execute function with sobel kernel
    virtual void execute(const Image& i);

private:

    // private kernel
    int kernelX[3][3];
    int kernelY[3][3];
};

#endif //PROJECT2_SOBELFILTER_H

void Sobel::execute(const Image &i) {
    // Resize the output to a similar size as the input image
    _output.resize(i.size());

    for (unsigned int t{0}; t < i.dim(4); ++t) {
        for (unsigned int c{0}; c < i.dim(3); ++c) {
            for (unsigned int z{0}; z < i.dim(2); ++z) {
                for (unsigned int y{0}; y < i.dim(1); ++y) {
                    for (unsigned int x{0}; x < i.dim(0); ++x) {

                        int sumY = 0;
                        int sumX = 0;

                        for (int kernel_x = -1; kernel_x < 2; ++kernel_x) {
                            for (int kernel_y = -1; kernel_y < 2; ++kernel_y) {

                                sumX += (kernelX[kernel_x+1][kernel_y+1] * i(x+kernel_x,y+kernel_y,z,c,t));
                                sumY += (kernelY[kernel_x+1][kernel_y+1] * i(x+kernel_x,y+kernel_y,z,c,t));

                            }
                        }
                        // Sobel of Gx and Gy. No division by kernel sum because they are 0
                        _output(x,y,z,t,c) = sqrt((pow(sumX,2) + pow(sumY,2)));
                    }
                }
            }
        }
    }
}