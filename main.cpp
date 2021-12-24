//
// The main calls the implemented pipelines
//
#pragma warning(default:4996)

#include "pipeline_one.h"

using namespace std;

int main() {

    GaussianSTDPipeline();

    SobelPipeline();

    ConvolutionalPipeline("basic");

    ConvolutionalPipeline("sharpen");

    ThresholdandMaskPipeline(600, "threshold", '>');

    return 0;
}

