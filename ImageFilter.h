//
// Created by Sinie van der Ben on 22/10/2021.
//

#ifndef PROJECT2_IMAGEFILTER_H
#define PROJECT2_IMAGEFILTER_H

#include <numeric>
#include <vector>
#include "image.h"
#include <cmath>

// Base class for all image filters to be implemented.

class ImageFilter{
public:
    // Constructor.
    ImageFilter() : _output(Image::dimension{ 0,0,0,0,0 }) {};

    // Destructor. This class does not own any free store allocated objects;but derived classes might, so it's virtual and empty
    virtual ~ImageFilter() {};

    // Because derived classes can add data/function/type members, we want to avoid copying (and thus slicing)
    ImageFilter(const ImageFilter&) = delete;
    ImageFilter& operator=(const ImageFilter&) = delete;

    // Set the input image for this filter
    void setInput(Image& i) {
        // The input should be const. However, we cannot store it that way in our base class. Therefor we cast away the const and store a pointer to the original data. Upon using the input data in update(), we cast it back.
        _input = const_cast<Image*>(&i);
    }

    // Get the input back as a const ref //todo: check if this goes well? const *T?
    const Image& getInput() const {
        // Didn't touch _input, cast it back to const;
        return const_cast<const Image&>(*_input);
    }

    // Get the output image result of this filter; available after calling update()
    Image getOutput() const { return _output; };

    // Update the image filter and compute the output
    virtual void update()
    {
        // Didn't touch _input, cast it back to const; then execute()
        execute(getInput());
    }

protected:
    // Container for the output image
    Image _output;

    // This method should be overloaded in your derived class and implement the
    // image filter that fills _output
    virtual void execute(const Image& i) = 0;

private:

    // Temporary storage of a pointer to the image data. Because we do some const-magic, we keep it private.
    Image* _input;

};

/*
class MaskedImageFilter : public ImageFilter{};
class StatisticsFilter: public ImageFilter{};
class ConvolutionalFilters: public ImageFilter{};
class GaussianBlur: public ImageFilter {};
class Sobel: public ImageFilter {};
*/
#endif //PROJECT2_IMAGEFILTER_H
