cmake_minimum_required(VERSION 3.20)
project(project2)

set(CMAKE_CXX_STANDARD 14)

add_executable(project2 main.cpp imagelib.h imagelib.cpp "ImageIOBase.h" ImageIOFactory.h ImageFilter.h image.h utils.h copy_save.h PIPImage.h MHDImage.h StatisticFilter.h ThresholdFilter.h MaskedFilter.h ConvolutionalFilter.h SobelFilter.h GaussianBlur.h pipeline_one.cpp)
