//
// Created by Sinie van der Ben on 05/11/2021.
//

#ifndef PROJECT2_MASKEDFILTER_H
#define PROJECT2_MASKEDFILTER_H

#include <numeric>
#include <vector>
#include "image.h"
#include <cmath>
#include "ImageFilter.h"

// Mask with two images: set output to 0 where mask is 0
class MaskedImageFilter : public ImageFilter
{
    typedef short T;
public:

    explicit MaskedImageFilter() : mask({0,0,0,0,0}), masked_part(0.0) {}

    // Set the random mask for this filter
    void setRandomMask(double part);

    // Set a pre made inputmask
    void setMask(Image img) {mask = img;};

    // Cast it to constant, just as the input. We don't want to change this.
    const Image& getMask() const {
        return const_cast<const Image&>(mask);
    }

protected:
    virtual void execute(const Image& i) override;

private:
    Image mask;
    double masked_part;
};

void MaskedImageFilter::setRandomMask(double part) {
    masked_part = part;
    mask.resize(getInput().size()); // Initializes the data of the mask with 0

    // Generate a random number between 0 and 1, depending on masked_part we will set the mask to zero.
    for(auto& m: mask){
        float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        double percentage = r*100;
        if(percentage < masked_part){m=0;};
    }
}

void MaskedImageFilter::execute(const Image &i) {

    // Clear and resize the output. Gives an empty image with 0
    _output.resize(i.size());

    // Get the mask
    const Image m = getMask();

    // Check if mask is of same size as the image
    if(i.size() == m.size()) {
        for (unsigned int c{0}; c < i.dim(4); ++c) {
            for (unsigned int t{0}; t < i.dim(3); ++t) {
                for (unsigned int z{0}; z < i.dim(2); ++z) {
                    for (unsigned int y{0}; y < i.dim(1); ++y) {
                        for (unsigned int x{0}; x < i.dim(0); ++x) {

                            if (m(x, y, z, t, c) == 0) { _output(x, y, z, t, c) = 0; }
                            else _output(x, y, z, t, c) = i(x, y, z, t, c);
                        }
                    }
                }
            }
        }
    }
    else error("Problem with masked filter: different size");
}




#endif //PROJECT2_MASKEDFILTER_H
