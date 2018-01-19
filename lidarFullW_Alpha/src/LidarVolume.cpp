// File name: LidarVolume.cpp
// Created on: 08-November-2017
// Author: ravi
 
#include "LidarVolume.hpp"
#include <math.h>

//Default constructor
LidarVolume::LidarVolume(){
  //initialize to zero
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

  bb_i_min = (int)(floor(ld_xMin)- 10);
  bb_j_min = (int)(floor(ld_yMin)- 10);
  bb_k_min = (int)(floor(ld_zMin)- 10);
  bb_i_max = (int)(ceil(ld_xMax)+ 10);
  bb_j_max = (int)(ceil(ld_yMax)+ 10);
  bb_k_max = (int)(ceil(ld_zMax)+ 10);

  i_extent = bb_i_max - bb_i_min;
  j_extent = bb_j_max - bb_j_min;
  k_extent = bb_k_max - bb_k_min;
}

void LidarVolume::insertPeak(peak *pulsePeak){
  
  peaks[currentPeak].time = pulsePeak->time;
  peaks[currentPeak].amp = pulsePeak->amp;
  peaks[currentPeak].width = pulsePeak->width;
  
  currentPeak++;
}

//allocate memory with a 3d array of the data
void LidarVolume::allocateMemory(){
  // we are going to allocate a 3D array of space that will hold peak 
  // information (we don't know how many per voxel)
  // vector<int>* element;
  // (vector<int>**) malloc (#ofiElem * #ofjElem * #ofk * sizeof(element));
}

void LidarVolume::deallocateMemory(){

}

int LidarVolume::position(int i, int j, int k){
  //return k + (j* len(k)) + (i*len(k)*len(j));
  return 0;
}
