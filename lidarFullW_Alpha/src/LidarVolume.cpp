/*
 * File name: LidarVolume.cpp
 * Created on: 08-November-2017
 * Author: ravi
 * Modified by: Nicholas Chapa
 */

#include "LidarVolume.hpp"


//Default constructor
LidarVolume::LidarVolume(){
  //initialize to zero
  bb_x_min = 0;
  bb_y_min = 0;
  bb_z_min = 0;
  bb_x_max = 0;
  bb_y_max = 0;
  bb_z_max = 0;

  bb_i_min = 0;
  bb_j_min = 0;
  bb_k_min = 0;
  bb_i_max = 0;
  bb_j_max = 0;
  bb_k_max = 0;

  i_extent = 0;
  j_extent = 0;
  k_extent = 0;
  
  currentPeak = 0;
  numOfPeaks = 5;
  //peakData = NULL;
  peaks = 0;
  
}

void LidarVolume::setBoundingBox(double ld_xMin, double ld_xMax,
                                       double ld_yMin, double ld_yMax,
                                       double ld_zMin, double ld_zMax){  

  bb_x_min = ld_xMin;
  bb_y_min = ld_yMin;
  bb_z_min = ld_zMin;
  bb_x_max = ld_xMax;
  bb_y_max = ld_yMax;
  bb_z_max = ld_zMax;

  bb_i_min = (int)(floor(bb_x_min)- 10);
  bb_j_min = (int)(floor(bb_y_min)- 10);
  bb_k_min = (int)(floor(bb_z_min)- 10);
  bb_i_max = (int)(ceil(bb_x_max)+ 10);
  bb_j_max = (int)(ceil(bb_y_max)+ 10);
  bb_k_max = (int)(ceil(bb_z_max)+ 10);

  i_extent = bb_i_max - bb_i_min;
  j_extent = bb_j_max - bb_j_min;
  k_extent = bb_k_max - bb_k_min;
}
/*
//allocate memory with a 3d array of the data
void LidarVolume::allocateMemory(){
  // we are going to allocate a 3D array of space that will hold peak 
  // information (we don't know how many per voxel)
  
  peakData = new double**[numOfPeaks];
  
  for(int i=0; i<numOfPeaks; i++){
  
  }
} */

//allcate memory with an array of structs addresses
void LidarVolume::allocateMemory(){
  peaks = new peak[numOfPeaks];
  
  //for(int i=0; i<numOfPeaks; i++){
    //peaks[i] = new peak;
  //}
}

void LidarVolume::insertPeak(struct peak *pulsePeak){
  
  peaks[currentPeak].time = pulsePeak->time;
  peaks[currentPeak].amp = pulsePeak->amp;
  peaks[currentPeak].width = pulsePeak->width;
  
  currentPeak++;
}

void LidarVolume::deallocateMemory(){
   //for(int i=0; i<numOfPeaks; i++) {
      delete peaks;
   //}
}
