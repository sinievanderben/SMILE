//
// Statistics filter: calculates statistical properties of the image such as the mean, median, min and max values, STD
//

#ifndef PROJECT2_STATISTICFILTER_H
#define PROJECT2_STATISTICFILTER_H

#include <numeric>
#include <vector>
#include "image.h"
#include <cmath>
#include "ImageFilter.h"

// todo: finish this

// statistics class
class StatisticsFilter: public ImageFilter{
    typedef short T;
public:

    // Get statistics
    T getMean() const{return mean;};
    T getMedian() const{return median;};
    T getMin() const {return min;};
    T getMax()const {return max;};
    pair<T, T> getMinMax();
    double getStandardDeviation(){return std;};

protected:

    // Inline functions to benefit from the speed

    // Set the number of voxels
    void setNumVox(Image& i){
        num_vox = calculateDimensions(i.size());
    }

// statistics functions
    void setMean(Image& i) {
        mean = accumulate(i.begin(), i.end(), 0.0) / num_vox;
    }

    void setMin(Image& i) {
        min = *(min_element(i.begin(), i.end()));
    }

    void setMax(Image& i) {
        max = *(max_element(i.begin(), i.end()));
    }

    void setMedian(Image& i){
        sort(i.begin(), i.end());

        // When even
        if (num_vox % 2 == 0) {median = *(i.begin() + (((num_vox - 1/2) + num_vox/2) /2));}
        // When odd
        else { median = *(i.begin() + num_vox / 2); }
    }

    void setStandardDeviation(Image& i){

        double variance = 0;
        for(auto & v: i){
            variance += (pow((v-mean),2)/num_vox);
        }
        std = sqrt(variance);

    }

    // Execute the statistic functions
    virtual void execute(const Image& i)
    {
        // Use copy, because we want to modify using Median sort()
        // If we want to use the image, we do not want to modify it
        Image copy_i(i);

        // First get number of voxels for the size
        setNumVox(copy_i);

        // Get all statistics
        setMean(copy_i);
        setMin(copy_i);
        setMax(copy_i);
        setMedian(copy_i);
        setStandardDeviation(copy_i);
    };

private:
    // Statistics declaration
    T mean;
    T median;
    T min;
    T max;
    int num_vox;
    double std;

};

// Get the min and the maximum value in a pair.
pair<short, short> StatisticsFilter::getMinMax() {
    pair<short, short> mima;
    mima.first = min;
    mima.second = max;
    return mima;
}

#endif //PROJECT2_STATISTICFILTER_H
