//
// The base class for reading and writing PIP and MHD files
// The input of this class are inputfilenames and this class outputs an object that can handle PIP or MHD files
// to read in the voxels of the image.
//

#ifndef PROJECT2_IMAGEIOBASE_H
#define PROJECT2_IMAGEIOBASE_H

#include<algorithm>
#include "image.h"
#include "utils.h"

class ImageIOBase {
public:

    static constexpr int N_DIM{5};

    // Pure virtual functions
    virtual Image read() = 0;
    virtual void write(const string &outfilename, Image img) = 0;

    // Destructor
    virtual ~ImageIOBase() = default;;

    // Delete copy constructor and assignment
    ImageIOBase(const ImageIOBase &) = delete;
    ImageIOBase &operator=(ImageIOBase &) = delete;

    protected:
        string fileName;

        ImageIOBase(){}; // can only be accessed by class derived and an ImageIOBase cannot be directly called
        explicit ImageIOBase(string fname);

        // Added function to easily get the voxels from an input stream
        void getVoxels(ifstream &ifs, int &num_voxels, Image &img);
        void readVoxels(ifstream &ifs, Image &img);

        // Added function to write voxels
        void writeVoxels(ofstream &ofs, Image &img);
    };

// Constructor with filename
ImageIOBase::ImageIOBase(string fname) {
    fileName = fname;
}

// Read in the bytes as shorts representing voxels for image data
void ImageIOBase::getVoxels(ifstream &ifs, int &num_voxels, Image &img) {

    int count = 0;
    auto p = img.begin();

    for (short x; ifs.read(as_bytes(x), sizeof(short));) {
        *p = x;
        p++;
        count++;
    }

    if (count != num_voxels) { error("Given dimensions and voxel number not equal"); }
}

// Read in the voxels of the image file
void ImageIOBase::readVoxels(ifstream &ifs, Image &img) {

    int num_vox = calculateDimensions(img.size());
    if (num_vox < 0 || num_vox == 0) throw "Negative dimension";

    getVoxels(ifs, num_vox, img);
}

void ImageIOBase::writeVoxels(ofstream &ofs, Image &img) {

    for (short & p : img) {
        ofs.write(as_bytes(p), sizeof(short));
    }
}

#endif //PROJECT2_IMAGEIOBASE_H
