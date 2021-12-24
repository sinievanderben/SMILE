//
// Thresholdfilter pre-implemented for the assignment. Changes to the direction of the threshold were made
//

#ifndef PROJECT2_THRESHOLDFILTER_H
#define PROJECT2_THRESHOLDFILTER_H

#include <numeric>
#include <vector>
#include "image.h"
#include <cmath>
#include "ImageFilter.h"

// Already implemented: thresholdfilter
class ThresholdImageFilter : public ImageFilter
{
    typedef short T;

public:
    // Constructor that initializes the threshold at 0
    ThresholdImageFilter() : _t(0), _d('>') {}

    // Get and set functions for the threshold value
    T getThreshold() const { return _t; }
    void setThreshold(T threshold) { _t = threshold; }
    void setDirection(char direction){_d = direction;}

protected:
    virtual void execute(const Image& i) override
    {
        // Clear and resize the output; resize returns new image with all 0
        _output.resize(i.size());

        // Bring the threshold parameter within scope, so it can be given to the lambda initializer
        const auto t = _t;
        const auto d = _d;

        // Do the thresholding with an std::transform, see Chapter 21

        // check this
        if (d == '>') {
            transform(i.begin(), i.end(), _output.begin(), [t](T value) { return value > t ? T(1) : T(0); });}
        else {
            transform(i.begin(), i.end(), _output.end(), [t](T value) { return value < t ? T(1) : T(0); });}
    }

private:
    // The threshold
    T _t;
    char _d;
};


#endif //PROJECT2_THRESHOLDFILTER_H
