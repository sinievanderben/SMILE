//
// Pipelines for the imaging library
//

#ifndef PROJECT2_PIPELINE_ONE_H
#define PROJECT2_PIPELINE_ONE_H

#include <string>

void GaussianSTDPipeline();

void SobelPipeline();

void ConvolutionalPipeline(const std::string& type);

void ThresholdandMaskPipeline(short threshold, std::string value, char direction);

#endif //PROJECT2_PIPELINE_ONE_H
