// File name: LidarVolume.cpp
// Created on: 08-November-2017
// Author: ravi

#ifndef LIDARVOLUME_HPP_
#define LIDARVOLUME_HPP_
#include <vector>
#include "Peak.hpp"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <png.h>
#include <cstring>
#include <string>

class LidarVolume{

  public:
    //The min and max fields describing the bounding box(bb) that
    //includes the first & last points of the sampled parts of the returning
    //waveforms of all pulses.
    double bb_x_min;
    double bb_y_min;
    double bb_z_min;
    double bb_x_max;
    double bb_y_max;
    double bb_z_max;

    int bb_i_min;
    int bb_j_min;
    int bb_k_min;
    int bb_i_max;
    int bb_j_max;
    int bb_k_max;

    double max_z;
    double min_z;

    //extent of x, y, and z as calculated from the pulse data (max - min)
    int i_extent;
    int j_extent;

    //number of peaks to be saved into
    int numOfPeaks;

    //current position in peaks array
    int currentPeak;

    //The raw raster data
    int* raster;

    std::vector<Peak>** volume;

    LidarVolume();

    //Read and store the mins and maxes from the header, calculate and store the
    //i, j,k values and the extents
    void setBoundingBox(double ld_xMin, double ld_xMax, double ld_yMin,
                        double ld_yMax, double ld_zMin, double ld_zMax);
    void insert_peak(Peak *peak);
    void allocateMemory();
    void deallocateMemory();
    int position(int i, int j);
    int gps_to_voxel_x(double x);
    int gps_to_voxel_y(double y);

    void rasterize();
    void rasterizeMaxElevation();
    void rasterizeMinElevation();
    int elev_high, elev_low;
    
    void display();


    int toTif(std::string filename, std::string geog_cs, int utm);


    // This takes the float value 'val', converts it to red, green &
    // blue values, then sets those values into the image memory buffer
    // location pointed to by 'ptr'
    void setRGB(unsigned char* r,unsigned char* g, unsigned char* b, float val);

    void writeImage(const char* filename, std::string geog_cs, int utm);



};


#endif /* LIDARVOLUME_HPP_ */

