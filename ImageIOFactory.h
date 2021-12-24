//
// Created by Sinie van der Ben on 22/10/2021.
//

#ifndef PROJECT2_IMAGEIOFACTORY_H
#define PROJECT2_IMAGEIOFACTORY_H

#include "PIPImage.h"
#include "MHDImage.h"

namespace Factory{
    class ImageIOFactory {
    public:
        static ImageIOBase *getIO(const string &filename);

        ImageIOFactory() = default;;
    };

    ImageIOBase *ImageIOFactory::getIO(const string &filename) {

        // Keys to assign the right ImageIOBase object to the image
        string pipKey = ".pip";
        string mhdKey = ".mhd";

        // Initialize the pointer
        ImageIOBase* base_p;

        if (filename.find(pipKey) != string::npos) {
            base_p = new PIPImageIO (filename); // Reserve space for the PIPImageIO object.
            return base_p;

        } else if (filename.find(mhdKey) != string::npos) {
            base_p = new MHDImageIO (filename); // Reserve space for the MHDImageIO object.
            return base_p;
        } else {
            cout << "Wrong extension";
            return nullptr;
        }
    }
}


#endif //PROJECT2_IMAGEIOFACTORY_H
