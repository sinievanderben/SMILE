//
// The class to handle the MHD files
//

#ifndef PROJECT2_MHDIMAGE_H
#define PROJECT2_MHDIMAGE_H

#include<algorithm>
#include "image.h"
#include "utils.h"
#include "ImageIOBase.h"

class MHDImageIO : public ImageIOBase {

    // Variables we need from MHD files
    Vector<int> NDIMS;
    vector<int> DIMSIZE;
    string RAWFILE;
    string ELEMENTTYPE;

public:
    explicit MHDImageIO(string fname); // constructor
    ~MHDImageIO() = default;; // destructor

    Image read() override;
    void write(const string &outfilename, Image img) override;

private:
    // Helper functions with reading
    vector<int> findStringDigits(string &input, const char *key); // Read in the digits in the MHD file
    string findStringNames(string &input); // Read in names in the MHD file
    string get_string(string &input, string keyword); // Get the lines of keywords
    //void readRaw(ifstream &ifs, Image &img); // Read the voxels of the .raw file

    // Helper functions with writing
    void writeRAW(const string &outfilename, Image img);
    void writeMHD(const string &outfilename, Image img);

};

// constructor
MHDImageIO::MHDImageIO(string fname) : ImageIOBase(fname) {
    fileName = fname;
}

// general method to find the digits after the '=', which is necessary for dimsize and ndims
vector<int> MHDImageIO::findStringDigits(string &input, const char *key) {

    string inputDigits = input.substr((input.find_last_of("=") + 2));

    // Key depends on what we want: NDim or DimSize
    //NDims
    if (strcmp(key, "N") == 0) {
        vector<int> getDigits;
        int a;
        stringstream ss(inputDigits);
        ss >> a;

        if (a >= 1) {
            getDigits.push_back(a);
            return getDigits;}
        else error("Negative NDims");} // No negative NDims is allowed

    //DimSize
    else if (strcmp(key, "D") == 0) {

        // Initialize everything to ONE. They'll be overwritten when a value is found
        int a = 1;
        int b = 1;
        int c = 1;
        int d = 1;
        int e = 1;
        stringstream ss(inputDigits);
        ss >> a >> b >> c >> d >> e;
        vector<int> getDigits{a, b, c, d, e};

        // No negatives allowed in dimensions
        if (all_of(getDigits.begin(), getDigits.end(), [](int i) { return i >= 0; })) return getDigits;
        else error("DimSize cannot be negative");}
    else error("wrong key");
}

// Find names in the stringstream
string MHDImageIO::findStringNames(string &input) {
    string inputNames = input.substr((input.find_last_of("=") + 2)); // with +2 we remove the whitespace and =

    string name;
    stringstream ss(inputNames);
    ss >> name;

    return name;
}

// Method to find the line of the desired information we want, independent of the place in the file
string MHDImageIO::get_string(string &input, string keyword) {

    size_t pos = input.find(keyword);
    string partialInput = input.substr(pos);
    size_t partialEnd = input.find_first_of('\n', pos);

    size_t content = partialEnd - pos;
    string end = partialInput.substr(0, content);

    return end;
}

Image MHDImageIO::read() {

    ifstream ifs{fileName};

    // Convert file to stringstream
    string contentMHD;
    if (ifs) {
        ostringstream ss;
        ss << ifs.rdbuf();
        contentMHD = ss.str();}

    // Get all information from the txt file
    // NDims
    string Ndims = get_string(contentMHD, "NDims");
    const char *d_Key = "N";
    NDIMS = findStringDigits(Ndims, d_Key); // check whether this is 5

    // Dimsize
    string Dimsize = get_string(contentMHD, "DimSize");
    const char *ds_Key = "D";
    DIMSIZE = findStringDigits(Dimsize, ds_Key);

    // Check if NDims and DimSize correspond
    auto count = count_if(DIMSIZE.begin(), DIMSIZE.end(), [&](auto const &val) { return val > 1; });
    if (count == NDIMS[0]) {

        // ElementType
        string Elementtype = get_string(contentMHD, "ElementType");
        ELEMENTTYPE = findStringNames(Elementtype);

        if (ELEMENTTYPE != "MET_SHORT") error("Not short type");

        // ElementDataFile
        string Elementdatafile = get_string(contentMHD, "ElementDataFile");
        RAWFILE = findStringNames(Elementdatafile);}

    else error("NDIMS and DIMSIZE not equal");

    // Get the .raw file
    ifstream ifs_raw{RAWFILE, ios::binary};
    if (!ifs_raw) error("can't open input file", fileName);

    // Create an Image with the dimensions and assign the voxel values
    array<int, N_DIM> dims = {DIMSIZE[0], DIMSIZE[1], DIMSIZE[2], DIMSIZE[3], DIMSIZE[4]};
    Image I(dims);

    // Read the .raw file and get the _data to the image
    readVoxels(ifs_raw, I);

    return I;
}

// Write the voxels of the .raw file into the data of the image
void MHDImageIO::writeRAW(const string &outfilename, Image img){

    // Open the outputfile .raw
    ofstream ofs{(outfilename + ".raw"), ios_base::binary};
    if (!ofs) error("can't open outputfile", outfilename);

    // Write the rest of the image to the file
    writeVoxels(ofs, img);

}

// Write the .mhd file, which is basically a copy
void MHDImageIO::writeMHD(const string &outfilename, Image img) {

    // Set up the output and the input file to copy the contents of .mhd file
    // We change nothing to the .mhd file, except the .raw name
    ofstream ofs{(outfilename + ".mhd")};
    if (!ofs) error("can't open outputfile ", outfilename);

    // Re-open another inputstream to copy contents
    ifstream ifs{fileName};
    if (!ifs) error("can't open inputfile ", outfilename);

    string str;

    if (ofs && ifs)
    {
        while (getline(ifs, str))
        {
            // Change the line of ElementDataFile only
            if(str == "ElementDataFile = " + RAWFILE) {
                ofs << "ElementDataFile = " + outfilename + ".raw";}

            else{ofs << str << '\n';}
        }
    }
    else error("cannot open both input and output file", outfilename);

    ifs.close();
    ofs.close();
}

// Write function
void MHDImageIO::write(const string &outfilename, Image img) {
    writeRAW(outfilename, img);
    writeMHD(outfilename, img);
}

#endif //PROJECT2_MHDIMAGE_H
