//
// Some helper functions used in other classes and filters
//

#ifndef PROJECT2_UTILS_H
#define PROJECT2_UTILS_H

#include "std_lib_facilities.h"

static constexpr int N_DIM{5};


using namespace std;

// Function overloading
int calculateDimensions(array<int, N_DIM> dimensions){

    int num_vox = dimensions[0];
    for (int i = 1; i < dimensions.size(); i++) {
        if (dimensions[i] > 0) { // If we multiply by 0, then it will all be 0.
            num_vox *= dimensions[i];
        }
    }
    return num_vox;
}

// Pixel lookup value: use clamping
int checkClamp(int coordinate, int maxRange){
    if(coordinate < 0){
        return 0;
    }
    else if(coordinate >= maxRange){
        return maxRange;
    }
    else return coordinate;
}

#endif //PROJECT2_UTILS_H
