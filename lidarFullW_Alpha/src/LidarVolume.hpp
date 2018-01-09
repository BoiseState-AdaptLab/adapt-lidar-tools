/*
 * File name: LidarVolume.cpp
 * Created on: 08-November-2017
 * Author: ravi
 */

#ifndef LIDARVOLUME_HPP_
#define LIDARVOLUME_HPP_

#include <math.h>

class LidarVolume{

  public:
    //The min and max fields describing the boundary box(bb) that includes the 
    //first & last points of the sampled parts of the returning waveforms of 
    //all pulses
    double bb_x_min;
    double bb_y_min;
    double bb_z_min;
    double bb_x_max;
    double bb_y_max;
    double bb_z_max;

    // integer mins
    int bb_i_min;
    int bb_j_min;
    int bb_k_min;
    // integer maxes
    int bb_i_max;
    int bb_j_max;
    int bb_k_max;

    //extent of x, y, and z as calculated from the pulse data (max - min)
    int i_extent;
    int j_extent;
    int k_extent;
    
    //number of peaks to be saved into 
    int numOfPeaks;
    
    //current position in peaks array
    int currentPeak;
    
    struct peak{
      double time;
      double amp;
      double width;
    };
    
    //array of pointers to structs for peak data
    peak *peaks;
    
    
    
    LidarVolume();

    //Read and store the mins and maxes from the header, calculate and store the
    //i, j,k values and the extents
    // void calculateBoundingBox(PULSEreader *pReader);
    void setBoundingBox(double ld_xMin, double ld_xMax, double ld_yMin,
                              double ld_yMax, double ld_zMin, double ld_zMax);
    void allocateMemory();
    void insertPeak(peak *pulsePeak);
    void deallocateMemory();


};


#endif /* LIDARVOLUME_HPP_ */

