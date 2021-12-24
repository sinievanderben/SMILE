//
// The class to handle PIP files
//

#ifndef PROJECT2_PIPIMAGE_H
#define PROJECT2_PIPIMAGE_H

#include<algorithm>
#include "image.h"
#include "utils.h"
#include "ImageIOBase.h"

class PIPImageIO : public ImageIOBase {
    // Important variables for PIP
    unsigned char type;
    int dim_x;
    int dim_y;
    int dim_z;
    int dim_c;
    int dim_t;

public:
    explicit PIPImageIO(string fname); // constructor
    ~PIPImageIO() = default;; // destructor

    Image read() override;
    void write(const string &outfilename, Image img) override;

};

// Constructor with filename
PIPImageIO::PIPImageIO(string fname) : ImageIOBase(fname) {
    fileName = fname;
}

// Read in the header and the voxels
Image PIPImageIO::read() {

    // get the stream + extension
    ifstream ifs(fileName, ios::binary);
    if (!ifs) error("can't open input file", fileName);

    // 21 byte header followed by binary image data

    // 1 byte: unsigned char datatype
    ifs.read(as_bytes(type), sizeof(unsigned char)); // check if it is 2 because we only want shorts
    if (type != 2) error("Bad type, not short (2)!");

    // 5 * 4 integer byte
    ifs.read(as_bytes(dim_x), sizeof(int));
    ifs.read(as_bytes(dim_y), sizeof(int));
    ifs.read(as_bytes(dim_z), sizeof(int));
    ifs.read(as_bytes(dim_c), sizeof(int));
    ifs.read(as_bytes(dim_t), sizeof(int));

    // Create the array with dimensions and get the corresponding vector
    array<int, N_DIM> dims = {dim_x, dim_y, dim_z, dim_c, dim_t};
    Image I(dims);

    // Read in the voxels
    readVoxels(ifs, I);

    return I;
}

void PIPImageIO::write(const string &outfilename, Image img) {

    ofstream ofs{outfilename + ".pip", ios_base::binary};
    if (!ofs) error("can't open outputfile", fileName);

    // Write the type
    ofs.write(as_bytes(2), sizeof(unsigned char));

    // Write the 5 integer dimensions
    ofs.write(as_bytes(img.dim(0)), sizeof(int)); // x
    ofs.write(as_bytes(img.dim(1)), sizeof(int)); // y
    ofs.write(as_bytes(img.dim(2)), sizeof(int)); // z
    ofs.write(as_bytes(img.dim(3)), sizeof(int)); // c
    ofs.write(as_bytes(img.dim(4)), sizeof(int)); // t

    writeVoxels(ofs, img);
}

#endif //PROJECT2_PIPIMAGE_H
