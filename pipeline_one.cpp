//
// 4 optional use cases of the imaging library: Gaussian, Sobel, Convolutional and Masked+Threshold
//
#include "std_lib_facilities.h"
#include "ImageIOBase.h"
#include "image.h"
#include "ImageIOFactory.h"
#include "pipeline_one.h"
#include "StatisticFilter.h"
#include "GaussianBlur.h"
#include "SobelFilter.h"
#include "MaskedFilter.h"
#include "ConvolutionalFilter.h"
#include <string>
#include "ThresholdFilter.h"

using namespace std;
using namespace Factory;

// Use the Gaussian filter with the STD as sigma
void GaussianSTDPipeline() {
    string filename;
    std::cout << "Please enter a PIP or MHD filename: " << endl;
    std::cin >> filename;

    unique_ptr<ImageIOBase> io{ImageIOFactory::getIO(filename)};

    Image image = io->read();

    // Account for the null pointer
    if (io != nullptr) {
        StatisticsFilter s;
        s.setInput(image);
        s.update();
        double std = s.getStandardDeviation(); // Get std as input for the Gaussian Blur

        GaussianBlur gb;
        gb.setInput(image);
        gb.setSigma(std);
        gb.update();
        image = gb.getOutput();

        io->write("gaussian", image);
    }
    else { error("No Image Object detected"); }
}

// Standard use of the Sobel edge detection filter
void SobelPipeline(){

    string filename;
    cout << "Please enter a PIP or MHD filename: " << endl;
    cin >> filename;

    unique_ptr<ImageIOBase> io{ImageIOFactory::getIO(filename)};

    Image image = io->read();

    // Account for the null pointer
    if (io != nullptr) {
        Sobel sb;
        sb.setInput(image);
        sb.update();
        image = sb.getOutput();

        io->write("sobel", image);
    }
    else { error("No Image Object detected"); }
}

// Two options of the convolutional filter: basic and sharpen
void ConvolutionalPipeline(const string& type){

    string filename;
    cout << "Please enter a PIP or MHD filename: " << endl;
    cin >> filename;

    unique_ptr<ImageIOBase> io{ImageIOFactory::getIO(filename)};

    Image image = io->read();

    // Account for the null pointer
    // Account for the null pointer
    if (io != nullptr) {
        ConvolutionalFilters c;
        c.setInput(image);

        // Function to generate a random mask based on a value between 0 and 10. Using a pre made mask is also possible with setMask
        c.setKernel(type);
        c.update();
        image = c.getOutput();

        io->write("conv_" + type, image);
    }
    else { error("No Image Object detected"); }
}

// Combination of the threshold and the masked filter
void ThresholdandMaskPipeline(short threshold, string value, char direction){

    string filename;
    cout << "Please enter a PIP or MHD filename: " << endl;
    cin >> filename;

    unique_ptr<ImageIOBase> io{ImageIOFactory::getIO(filename)};

    Image image = io->read();
    if (io != nullptr) {

        StatisticsFilter s;
        s.setInput(image);
        s.update();

        ThresholdImageFilter t;
        Image thresholdImage(image);
        t.setInput(thresholdImage);

        if(value == "mean"){
            short mean = s.getMean();
            t.setThreshold(mean);
        }
        else if(value =="median"){
            short median = s.getMedian();
            t.setThreshold(median);}
        else{
            t.setThreshold(threshold);
        }
        t.setDirection(direction);
        t.update();
        thresholdImage = t.getOutput();

        MaskedImageFilter m;
        m.setInput(image);
        m.setMask(thresholdImage);
        m.update();
        image = m.getOutput();

        io->write("threshold_mask", image);
    }
    else { error("No Image Object detected"); }
}



