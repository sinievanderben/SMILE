//
// Created by Sinie van der Ben on 02/11/2021.
//

#ifndef PROJECT2_COPY_SAVE_H
#define PROJECT2_COPY_SAVE_H
//
// Created by Sinie van der Ben on 22/10/2021.
//

#ifndef PROJECT2_IMAGEFILTER_H
#define PROJECT2_IMAGEFILTER_H

#include <numeric>
#include <vector>
#include "image.h"

// Base class for all image filters to be implemented.

class ImageFilter{
    typedef short T; // all we do is short
public:

    // Constructor.
    ImageFilter() {};

    // Destructor. This class does not own any free store allocated objects;but derived classes might, so it's virtual and empty
    virtual ~ImageFilter() {};

    // Because derived classes can add data/function/type members, we want to avoid copying (and thus slicing)
    ImageFilter(const ImageFilter&) = delete;
    ImageFilter& operator=(const ImageFilter&) = delete;

    // Set the input image for this filter
    void setInput(Image& i) {
        // The input should be const. However, we cannot store it that way in our base class. Therefor we cast away the const and store a pointer to the original data. Upon using the input data in update(), we cast it back.
        _input = i;
    }

    // Get the input back as a const ref
    const vector<T>& getInput() const {
        // Didn't touch _input, cast it back to const;
        return const_cast<const vector<T>&>(*_input);
    }

    // Get the output image result of this filter; available after calling update()
    vector<T> getOutput() const { return _output; };

    // Update the image filter and compute the output
    virtual void update()
    {
        // Didn't touch _input, cast it back to const; then execute()
        execute(getInput());
    }

protected:
    // Container for the output image
    vector<T> _output;

    // This method should be overloaded in your derived class and implement the
    // image filter that fills _output
    virtual void execute(const vector<T>& i) = 0;

private:

    // Temporary storage of a pointer to the image data. Because we do some const-magic, we keep it private.
    vector<T>* _input;

};

//// OWN FILTERS
//TODO: fix convolutional filter
//TODO: add own filter --> Gradient blur
//TODO: add a second own filter? --> SOBEL
//TODO: otsu threshold
//TODO: change to work for image

// statistics class
class StatisticsFilter: public ImageFilter{
    typedef short T; // all we do is short
public:

    // get statistics
    double getMean(){return mean;};
    short getMedian(){return median;};
    short getMin() {return min;};
    short getMax(){return max;};
    pair<short, short> getMinMax();

protected:

// statistics functions
    void setMean(vector<T>& i) {
        mean = accumulate(i.begin(), i.end(), 0) / i.size();
    }

    void setMin(vector<T>& i) {
        min = *(min_element(i.begin(), i.end()));
    }

    void setMax(vector<T>& i) {
        max = *(max_element(i.begin(), i.end()));
    }

    void setMedian(vector<T>& i){
        sort(i.begin(), i.end());

        if (i.size() % 2 == 0)           // even
            median = (i[i.size() / 2 - 1] + i[i.size() / 2]) / 2;
        else                                // odd
            median = i[i.size() / 2];
    }


    // execute the statistic functions
    virtual void execute(const vector<T>& i)
    {
        vector<T> copy_i;
        copy(i.begin(), i.end(), back_inserter(copy_i));

        // get all statistics
        setMean(copy_i);
        setMin(copy_i);
        setMax(copy_i);
        setMedian(copy_i);
    };

private:
    // statistics
    double mean;
    short median;
    short min;
    short max;

};

pair<short, short> StatisticsFilter::getMinMax() {
    pair<short, short> mima;
    mima.first = min;
    mima.second = max;
    return mima;
}

class ThresholdImageFilter : public ImageFilter
{
    typedef short T;

public:
    // Constructor that initializes the threshold at 0
    ThresholdImageFilter() : _t(0) {}

    // Get and set functions for the threshold value
    T getThreshold() const { return _t; }
    void setThreshold(T threshold) { _t = threshold; }

protected:
    virtual void execute(const vector<T>& i) override
    {
        // Clear and resize the output
        _output.clear();
        _output.resize(i.size());

        // Bring the threshold parameter within scope, so it can be given to the lambda initializer
        const auto t = _t;

        // Do the thresholding with an std::transform, see Chapter 21
        transform(begin(i), end(i), begin(_output), [t](T value) { return value > t ? T(1) : T(0); }

        );
    }

private:
    // The threshold
    T _t;
};


// Mask with two images: set output to 0 where mask is 0
class MaskedImageFilter : public ImageFilter
{
    typedef short T;
public:

    // Set the input mask for this filter
    void setMask(const vector<T>& m) {
        mask = const_cast<vector<T>* >( &m );
    }

    const vector<T>& getMask() const {
        // Didn't touch _input, cast it back to const;
        return const_cast<const vector<T>&>(*mask);
    }

protected:
    virtual void execute(const vector<T>& i) override {

        // Clear and resize the output
        _output.clear();
        _output.resize(i.size());

        const vector<T> m = getMask();

        // check if mask is of same size
        try{
            if(i.size() == m.size()){
                for(vector<T>::size_type x = 0; x<i.size(); x++){
                    if(m[x] == 0) { // if m at that place is 0, insert 0 in output
                        _output[x] = 0;
                    }
                    else { // else insert the value of i in the output
                        _output[x] = i[x];
                    }
                }
                cout << "succes";
            }
            else throw "Image and mask size not equal";
        }
        catch(const char* expr){
            cout << "Error in Masked Filter: " << expr;
        }
    }

private:
    vector<T>* mask;
};

// convolutional filter
class ConvolutionalFilter: public ImageFilter{
    typedef short T; // all we do is short
public:

    //TODO: information needed on
    // image width and image height (and other dimensions?)
    // we get a vector, but convolution works in a matrix --> how?

    // constructor to initialize a kernel of 3x3
    ConvolutionalFilter() : kernel() {}

    // get a kernel
    // get a kernel function


    // set kernel
    void setKernel(const vector<T>& k){kernel = const_cast<vector<T>* >( &k );}
    //const vector<T>& getKernel() const;


    // Update the image filter and compute the output
    virtual void update()
    {
        // Didn't touch _input, cast it back to const; then execute()
        execute(getInput());
    }

protected:

    // convolution functions

    // execute function with kernel
    virtual void execute(const vector<T>& i) = 0;

private:

    // Temporary storage of a pointer to the image data. Because we do some const-magic, we keep it private.
    vector<T>* kernel;

};


// Gaussian Blur

// Sobel


#endif //PROJECT2_IMAGEFILTER_H

#endif //PROJECT2_COPY_SAVE_H
