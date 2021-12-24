//
// Convolutional filter: with set size of 3 x 3, but choice between 'basic'  or 'sharpen'  kernels
//

#ifndef PROJECT2_CONVOLUTIONALFILTER_H
#define PROJECT2_CONVOLUTIONALFILTER_H

#include <numeric>
#include <vector>
#include "image.h"
#include <cmath>
#include "ImageFilter.h"

// Convolutional filter
class ConvolutionalFilters: public ImageFilter{
    typedef short T; // all we do is short
public:

    // Constructor to initialize a basic kernel of 3x3
    ConvolutionalFilters() : kernel{-1,0,1,-1,0,1,-1,0,1}, kernelType{"basic"}{}

    void setKernel(string kernelname) {
        if(kernelname == "sharpen"){
            kernelType = kernelname;
            int new_kernel [] = {0,-1,0,-1,5,-1,0,-1,0};

            // Copy the values of the other kernel into the kernelobject.
            memcpy(kernel, new_kernel, 9*sizeof(int));
        }
        else{
            kernelType = "basic";
        }
    }

    // Update the image filter and compute the output
    virtual void update()
    {
        execute(getInput());
    }

protected:
    virtual void execute(const Image& i);

private:

    // Private kernel and kerneltype
    int kernel[3][3];
    string kernelType;

};

void ConvolutionalFilters::execute(const Image &i) {
    _output.resize(i.size());

    for (unsigned int t{0}; t < i.dim(4); ++t) {
        for (unsigned int c{0}; c < i.dim(3); ++c) {
            for (unsigned int z{0}; z < i.dim(2); ++z) {
                for (unsigned int y{0}; y < i.dim(1); ++y) {
                    for (unsigned int x{0}; x < i.dim(0); ++x) {

                        int sum = 0; // The sum might not fit into a short.

                        for (int kernel_x = -1; kernel_x < 2; ++kernel_x) {
                            for (int kernel_y = -1; kernel_y < 2; ++kernel_y) {

                                sum = sum + kernel[kernel_x+1][kernel_y+1] * i(x+kernel_x, y+kernel_y, z, c,t);
                            }
                        }
                        // No need for division, because sum is either 0 for basic or 1 for sharpen
                        _output(x, y, z, t, c) = sum; // Division makes this a short again, which is nice.
                    }
                }
            }
        }
    }

}

#endif //PROJECT2_CONVOLUTIONALFILTER_H
