//
// File for the Image object
//

#ifndef PROJECT2_IMAGE_H
#define PROJECT2_IMAGE_H

#include "utils.h"
#include "std_lib_facilities.h"

class Image
{
public:
    // Internally used data types
    typedef short T;
    typedef std::array<int, N_DIM> dimension;

    // STL container and iterator typedefs
    typedef T         value_type;
    typedef T*        iterator;
    typedef const T*  const_iterator;
    typedef T&        reference;
    typedef const T&  const_reference;

    // Constructors and destructor
    explicit Image(dimension);
    Image(const Image&); // Copy constructor
    Image(Image&&); // Move constructor

    virtual ~Image() = default;;

    // Assignment operators
    Image& operator=(const Image&); // Copy
    Image& operator=(Image&&); // Move

    // Iterators
    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    // Size and resize
    dimension size() const;                 // the dimension object
    unsigned int dim(unsigned int d) const; // get the size of a dimension
    unsigned int nr_dims() const;           // number of dimensions > 1
    void resize(const dimension& d);        // resize this image

    // Pixel value lookup
    value_type operator()(int x = 0, int y = 0, int z = 0, int c = 0, int t = 0) const;
    reference operator()(int x = 0, int y = 0, int z = 0, int c = 0, int t = 0);

private:
    dimension _dimensions;
    T* _data; // data is a pointer, just as with the image filters
};

// Paramaterized constructor
Image:: Image(dimension d) {
    _dimensions = d;

    int data_size = calculateDimensions(_dimensions);
    _data = new T[data_size];
}

// Copy constructor
Image::Image(const Image & i){

    _dimensions = i._dimensions;

    int num_vox = calculateDimensions(i._dimensions);
    _data = new T[num_vox];

    copy(i.begin(), i.end(), _data);
}

// Move constructor
Image::Image(Image&& i):_dimensions{i._dimensions}, _data{i._data} {
    i._dimensions = {0,0,0,0,0}; // Set array to 0 because it cannot be deleted
    i._data = nullptr;
}

// Return a pointer to the first element in image
Image::iterator Image::begin() {
    return _data;
}

// Return a pointer to one past the last element
Image::iterator Image::end() {
    int num_voxels = calculateDimensions(_dimensions);
    return next(_data, num_voxels);
}

// Return a constant pointer
Image::const_iterator Image::begin() const {
    return _data;
}

// Return a constant pointer
Image::const_iterator Image::end() const {
    int num_voxels = calculateDimensions(_dimensions);
    return next(_data, num_voxels);
}

// Return the current size, which is the dimension object
Image::dimension Image::size() const {
    return _dimensions;
}

// Get the number of higher than 1 dimensions
unsigned int Image::nr_dims() const {
    int count = 0 ;
    for(int i = 0; i < sizeof(_dimensions); i++){
        if(_dimensions[i] > 1) count ++;
    }
    return count;
}

// Get the size of a dimension
unsigned int Image::dim(unsigned int d) const {

    switch(d){
        case 0:
            return _dimensions[0];
        case 1:
            return _dimensions[1];
        case 2:
            return _dimensions[2];
        case 3:
            return _dimensions[3];
        case 4:
            return _dimensions[4];
        default:
            error("not a dimension");
    }
}

void Image::resize(const Image::dimension &d) {

    int num_vox = calculateDimensions(d);

    // Reserve new space and delete the old space
    T* resized_data = new T[num_vox];
    delete[] _data;

    // Initialize the new values with 0
    for(int x=0; x<num_vox; ++x) resized_data[x] = 0;

    _data = resized_data;
    _dimensions = d;
}

Image::value_type Image::operator()(int x, int y, int z, int c, int t) const {

    // check for negative and out of range
    int coord_x = checkClamp(x, _dimensions[0]);
    int coord_y = checkClamp(y, _dimensions[1]);
    int coord_z = checkClamp(z, _dimensions[2]);
    int coord_c = checkClamp(c, _dimensions[3]);
    int coord_t = checkClamp(t, _dimensions[4]);

    return _data[coord_x + (coord_y*dim(0)) + (coord_z*dim(0)*dim(1)) + (coord_c*dim(0)*dim(1)*dim(2)) + (coord_t*dim(0)*dim(1)*dim(2)*dim(3))];
}

Image::reference Image::operator()(int x, int y, int z, int c, int t) {

    // check for negative and out of range
    int coord_x = checkClamp(x, _dimensions[0]);
    int coord_y = checkClamp(y, _dimensions[1]);
    int coord_z = checkClamp(z, _dimensions[2]);
    int coord_c = checkClamp(c, _dimensions[3]);
    int coord_t = checkClamp(t, _dimensions[4]);

    return _data[coord_x + (coord_y*dim(0)) + (coord_z*dim(0)*dim(1)) + (coord_c*dim(0)*dim(1)*dim(2)) + (coord_t*dim(0)*dim(1)*dim(2)*dim(3))];
}


// Copy assignment
Image &Image::operator=(const Image& i) {
    
    // Account for self referencing: nothing has to be done 
    if(this==&i) return *this;

    // Create new
    int num_vox = calculateDimensions(i._dimensions);
    T* copy_i = new T[num_vox];

    // Copy old
    for(int x=0; x<num_vox; ++x) copy_i[x] = i._data[x];

    // Delete old and assign new
    delete[] _data;
    _data = copy_i;
    _dimensions = i._dimensions;

    return *this;
}

// Move assignment
Image &Image::operator=(Image && i) {

    // Delete old
    delete[] _data;

    // Copy new
    _data = i._data;
    _dimensions = i._dimensions;

    // Empty old
    i._data = nullptr;
    i._dimensions = {0,0,0,0,0}; // delete[]?

    return *this;
}

#endif //PROJECT2_IMAGE_H
